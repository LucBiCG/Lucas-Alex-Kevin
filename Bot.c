#include "Bot.h"

#include "map.h"

struct Bot* CreateBot()
{
    struct Bot* bot = (struct Bot*)malloc(sizeof(struct Bot));
    
    bot->position = (sfVector2i){0, 0};
    
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
    if (!bot->sprite) return;
    sfSprite_destroy(bot->sprite);
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
    
    switch (direction)
    {
    case NORTH:
        if (newPosition.y > 0)
        {
            newPosition.y -= distance;
        }
        break;
    case EAST:
        if (newPosition.x < (GRID_COLS - 1))
        {
            newPosition.x += distance;
        }
        break;
    case SOUTH:
        if (newPosition.y < (GRID_ROWS - 1))
        {
            newPosition.y += distance;
        }
        break;
    case WEST:
        if (newPosition.x > 0)
        {
            newPosition.x -= distance;
        }
        break;
    default:
        break;
    }

    enum CellType destinationCellType = grid->cell[newPosition.y][newPosition.x]->type;

    if (destinationCellType != OBSTACLE)
    {
        bot->position = newPosition;
        sfVector2f newSpritePosition = sfSprite_getPosition(grid->cell[bot->position.y][bot->position.x]->sprite);
        newSpritePosition.x += 5.f;
        newSpritePosition.y += 5.f;
        sfSprite_setPosition(bot->sprite, newSpritePosition);
    } else
    {
        printf("can't go there ! \n");
    }

    switch (destinationCellType)
    {
    case END:
        return REACH_END;
    case EMPTY:
        return FAILURE;
    case START:
    case WALKABLE:
    case OBSTACLE:
    default:
        return NOTHING;
    }
}

void AddMovement(struct Bot* bot, enum MovementType type, enum Direction direction)
{
    if (!bot) return;
    // Add a new element Move to bot's MoveQueue
    int currentLength = 0;
    while (bot->MoveQueue[currentLength].type == MOVE_TO || bot->MoveQueue[currentLength].type == JUMP)
    {
        currentLength++;
    }
    bot->MoveQueue[currentLength].type = type;
    bot->MoveQueue[currentLength].direction = direction;
}

void MoveBot_AI(struct GameData* data)
{
    if (!data->bot || !data->grid) return;
    
    while (data->bot->MoveQueue[data->step].type == MOVE_TO || data->bot->MoveQueue[data->step].type == JUMP)
    {
        sfSleep(sfMilliseconds(500));
        enum MovementType type = data->bot->MoveQueue[data->step].type;
        enum Direction direction = data->bot->MoveQueue[data->step].direction;
        (data->step)++;
        data->pathResult = MoveBot(data->bot, data->grid, type, direction);
    }
}

bool SearchPath_AI(struct Bot* bot, Grid* grid)
{
    // Implement pathfinding algorithm to fill bot's MoveQueue
    return false;
}

