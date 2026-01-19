#include "Bot.h"
#include "map.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Bot* CreateBot()
{
    struct Bot* bot = (struct Bot*)malloc(sizeof(struct Bot));
    
    bot->position = (sfVector2i){ 0, 0 };
    bot->queueCapacity = 1000;
    bot->queueSize = 0;
    bot->MoveQueue = (struct Move*)malloc(bot->queueCapacity * sizeof(struct Move));
    
    bot->sprite = sfSprite_create();
    sfTexture* tex = sfTexture_createFromFile("./Assets/Characters/Bot01.png", NULL);
    sfSprite_setTexture(bot->sprite, tex, sfTrue);
    sfSprite_setPosition(bot->sprite, (sfVector2f){0, 0});
    float scale = ((float)CELL_SIZE / 24.f) * 0.75f;
    sfSprite_setScale(bot->sprite, (sfVector2f){scale, scale});
    
    return bot;
}

void SpawnBotAtStartCell(struct Bot* bot, Grid* grid)
{
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            if (grid->cell[i][j]->type == START)
            {
                bot->position = grid->cell[i][j]->coord;
                sfVector2f startCelPosition = sfSprite_getPosition(grid->cell[i][j]->sprite);
                startCelPosition.x += 5.f;
                startCelPosition.y += 5.f;
                sfSprite_setPosition(bot->sprite, startCelPosition);
                break;
            }
        }
    }
    
}

void DestroyBot(struct Bot* bot)
{
    if (!bot) return;
    if (bot->sprite) sfSprite_destroy(bot->sprite);
    if (bot->MoveQueue) free(bot->MoveQueue);
    free(bot);
}

void DrawBot(sfRenderWindow* window, struct Bot* bot)
{
    if (!window || !bot || !bot->sprite) return;
    sfRenderWindow_drawSprite(window, bot->sprite, NULL);
}

int MoveBot(struct Bot* bot, Grid* grid, enum MovementType type, enum Direction direction)
{
    int distance = 1;
    if (type == JUMP) distance = 2;

    sfVector2i newPosition = bot->position;

    // Calcul de la nouvelle position
    switch (direction)
    {
    case NORTH:
        if (newPosition.y >= distance)
            newPosition.y -= distance;
        else
            return FAILURE;
        break;
    case EAST:
        if (newPosition.x < (GRID_COLS - distance))
            newPosition.x += distance;
        else
            return FAILURE;
        break;
    case SOUTH:
        if (newPosition.y < (GRID_ROWS - distance))
            newPosition.y += distance;
        else
            return FAILURE;
        break;
    case WEST:
        if (newPosition.x >= distance)
            newPosition.x -= distance;
        else
            return FAILURE;
        break;
    default:
        return FAILURE;
    }

    // IMPORTANT: Pour un JUMP, on peut atterrir sur un OBSTACLE (type 4) 
    // si c'est ce que l'algorithme a prévu
    enum CellType destinationCellType = grid->cell[newPosition.y][newPosition.x]->type;

    // Logique de validation du mouvement
    if (type == JUMP) {
        // Pour un saut, on peut atterrir sur WALKABLE, START, END... 
        // ET SUR OBSTACLE (type 4) si c'est une cellule de saut spéciale
        if (destinationCellType == EMPTY) {
            printf("Can't jump to EMPTY cell!\n");
            return FAILURE;
        }
    }
    else {
        // Pour un MOVE_TO normal, on ne peut pas aller sur OBSTACLE
        if (destinationCellType == OBSTACLE || destinationCellType == EMPTY) {
            printf("Can't move to OBSTACLE or EMPTY cell!\n");
            return FAILURE;
        }
    }

    // Si on arrive ici, le mouvement est valide
    bot->position = newPosition;
    sfVector2f newSpritePosition = sfSprite_getPosition(grid->cell[bot->position.y][bot->position.x]->sprite);
    newSpritePosition.x += 5.f;
    newSpritePosition.y += 5.f;
    sfSprite_setPosition(bot->sprite, newSpritePosition);

    // Vérifier si on a atteint la fin
    if (destinationCellType == END) {
        return REACH_END;
    }

    return NOTHING;
}

void AddMovement(struct Bot* bot, enum MovementType type, enum Direction direction)
{
    if (!bot) return;
    // Add a new element Move to bot's MoveQueue
    if (bot->queueSize >= bot->queueCapacity) {
        printf("Erreur: MoveQueue pleine\n");
        return;
    }

    bot->MoveQueue[bot->queueSize].type = type;
    bot->MoveQueue[bot->queueSize].direction = direction;
    bot->queueSize++;
}

void MoveBot_AI(struct GameData* data)
{
    if (!data->bot || !data->grid) return;

    // Réinitialiser l'étape
    data->step = 0;

    // Exécuter les mouvements jusqu'à la fin
    while (data->step < data->bot->queueSize &&
        data->bot->MoveQueue[data->step].type >= 0) {

        sfSleep(sfMilliseconds(500));

        enum MovementType type = data->bot->MoveQueue[data->step].type;
        enum Direction direction = data->bot->MoveQueue[data->step].direction;

        data->pathResult = MoveBot(data->bot, data->grid, type, direction);
        data->step++;

        // Si on a atteint la fin, arrêter
        if (data->pathResult == REACH_END) {
            printf("Arrivée atteinte !\n");
            break;
        }
    }
}

typedef struct QueueNode {
    sfVector2i position;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

static Queue* CreateQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

static void Enqueue(Queue* q, sfVector2i pos) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->position = pos;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

static sfVector2i Dequeue(Queue* q) {
    if (q->front == NULL) return (sfVector2i) { -1, -1 };

    QueueNode* temp = q->front;
    sfVector2i pos = temp->position;
    q->front = q->front->next;

    if (q->front == NULL) q->rear = NULL;

    free(temp);
    q->size--;
    return pos;
}

static bool IsQueueEmpty(Queue* q) {
    return q->front == NULL;
}

static void FreeQueue(Queue* q) {
    while (!IsQueueEmpty(q)) Dequeue(q);
    free(q);
}

// Conversion direction vers vecteur
static sfVector2i DirectionToVector(enum Direction dir) {
    switch (dir) {
    case NORTH: return (sfVector2i) { 0, -1 };
    case EAST:  return (sfVector2i) { 1, 0 };
    case SOUTH: return (sfVector2i) { 0, 1 };
    case WEST:  return (sfVector2i) { -1, 0 };
    default:    return (sfVector2i) { 0, 0 };
    }
}

// Conversion vecteur vers direction
static enum Direction VectorToDirection(sfVector2i from, sfVector2i to) {
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    // Pour les sauts, dx ou dy peut être ±2
    // Normaliser à ±1 pour la direction
    if (dx > 0) dx = 1;
    if (dx < 0) dx = -1;
    if (dy > 0) dy = 1;
    if (dy < 0) dy = -1;

    if (dx == 1 && dy == 0) return EAST;
    if (dx == -1 && dy == 0) return WEST;
    if (dx == 0 && dy == 1) return SOUTH;
    if (dx == 0 && dy == -1) return NORTH;

    printf("ERREUR VectorToDirection: dx=%d, dy=%d\n", dx, dy);
    return NORTH; // Direction par défaut
}

bool SearchPath_AI(struct Bot* bot, Grid* grid)
{
    if (!bot || !grid) return false;

    // 1. Trouver les positions de départ et d'arrivée
    sfVector2i start = { -1, -1 };
    sfVector2i end = { -1, -1 };

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (grid->cell[y][x]->type == START) start = (sfVector2i){ x, y };
            if (grid->cell[y][x]->type == END) end = (sfVector2i){ x, y };
        }
    }

    if (start.x == -1 || end.x == -1) {
        printf("Erreur: Départ ou arrivée non trouvé\n");
        return false;
    }

    printf("Départ: (%d, %d), Arrivée: (%d, %d)\n", start.x, start.y, end.x, end.y);

    // 2. Initialisation BFS
    bool visited[20][20] = { {false} };
    sfVector2i parent[20][20];
    enum MovementType moveType[20][20];

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            parent[i][j] = (sfVector2i){ -1, -1 };
            moveType[i][j] = MOVE_TO;
        }
    }

    // Directions
    sfVector2i directions[4] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };

    // 3. BFS
    Queue* queue = CreateQueue();
    Enqueue(queue, start);
    visited[start.y][start.x] = true;

    bool found = false;

    while (!IsQueueEmpty(queue)) {
        sfVector2i current = Dequeue(queue);

        // Si on a trouvé la sortie
        if (current.x == end.x && current.y == end.y) {
            found = true;
            break;
        }

        // Explorer les 4 directions
        for (int i = 0; i < 4; i++) {
            sfVector2i next = (sfVector2i){
                current.x + directions[i].x,
                current.y + directions[i].y
            };

            // Vérifier les limites
            if (next.x < 0 || next.x >= 20 || next.y < 0 || next.y >= 20) {
                continue;
            }

            enum CellType nextCellType = grid->cell[next.y][next.x]->type;

            // OPTION 1: Déplacement normal (MOVE_TO)
            if (nextCellType == WALKABLE || nextCellType == START || nextCellType == END) {
                if (!visited[next.y][next.x]) {
                    visited[next.y][next.x] = true;
                    parent[next.y][next.x] = current;
                    moveType[next.y][next.x] = MOVE_TO;
                    Enqueue(queue, next);
                }
                continue;
            }

            // OPTION 2: OBSTACLE (type 4) - on peut sauter par-dessus
            if (nextCellType == OBSTACLE) {
                // Calculer la case après l'obstacle (JUMP de 2 cases)
                sfVector2i jumpTarget = (sfVector2i){
                    current.x + directions[i].x * 2,
                    current.y + directions[i].y * 2
                };

                // Vérifier les limites
                if (jumpTarget.x < 0 || jumpTarget.x >= 20 ||
                    jumpTarget.y < 0 || jumpTarget.y >= 20) {
                    continue;
                }

                // Vérifier si déjà visité
                if (visited[jumpTarget.y][jumpTarget.x]) {
                    continue;
                }

                // La case d'arrivée doit être accessible
                enum CellType targetType = grid->cell[jumpTarget.y][jumpTarget.x]->type;
                if (targetType == WALKABLE || targetType == START || targetType == END) {
                    visited[jumpTarget.y][jumpTarget.x] = true;
                    parent[jumpTarget.y][jumpTarget.x] = current;
                    moveType[jumpTarget.y][jumpTarget.x] = JUMP;
                    Enqueue(queue, jumpTarget);
                }
                continue;
            }

            // OPTION 3: EMPTY - inaccessible
            if (nextCellType == EMPTY) {
                continue;
            }
        }
    }

    // 4. Reconstruction du chemin
    if (found) {
        printf("Chemin trouvé ! Reconstruction...\n");

        sfVector2i path[400];
        enum MovementType pathMoveTypes[400];
        int pathLength = 0;
        sfVector2i current = end;

        // Reconstruire à l'envers
        while (!(current.x == start.x && current.y == start.y)) {
            path[pathLength] = current;
            pathMoveTypes[pathLength] = moveType[current.y][current.x];
            pathLength++;

            current = parent[current.y][current.x];

            if (pathLength >= 400) break;
        }
        path[pathLength] = start;
        pathMoveTypes[pathLength] = MOVE_TO;
        pathLength++;

        printf("Longueur du chemin: %d mouvements\n", pathLength - 1);

        // DEBUG: Afficher le chemin
        printf("\n=== CHEMIN TROUVÉ ===\n");
        for (int i = pathLength - 1; i >= 0; i--) {
            printf("(%d,%d) %s\n", path[i].x, path[i].y,
                (pathMoveTypes[i] == JUMP) ? "[JUMP]" : "");
        }

        // 5. Remplir la MoveQueue
        bot->queueSize = 0;

        for (int i = pathLength - 2; i >= 0; i--) {
            sfVector2i from = path[i + 1];
            sfVector2i to = path[i];

            enum Direction dir = VectorToDirection(from, to);
            enum MovementType type = pathMoveTypes[i];

            if (bot->queueSize >= bot->queueCapacity) {
                printf("Erreur: MoveQueue pleine\n");
                break;
            }

            bot->MoveQueue[bot->queueSize].type = type;
            bot->MoveQueue[bot->queueSize].direction = dir;
            bot->queueSize++;

            printf("Mouvement ajouté: de (%d,%d) à (%d,%d) dir: %d, type: %s\n",
                from.x, from.y, to.x, to.y, dir,
                (type == JUMP) ? "JUMP" : "MOVE_TO");
        }

        if (bot->queueSize < bot->queueCapacity) {
            bot->MoveQueue[bot->queueSize].type = -1;
            bot->queueSize++;
        }

        FreeQueue(queue);
        return true;
    }

    printf("Aucun chemin trouvé\n");
    FreeQueue(queue);
    return false;
}