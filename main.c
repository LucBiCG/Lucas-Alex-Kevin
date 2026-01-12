#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include "basics.h"
#include "Bot.h"
#include "grid.h"
#include "map.h"
#include "MapSelectionMenu.h"


int main(void)
{
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow* window;
    sfEvent event;

    /* Create the main window */
    window = sfRenderWindow_create(mode, WINDOW_TITLE, sfClose, NULL);
    if (!window)
    {
        return NULL_WINDOW;
    }
    
    MapSelectionMenu* mapSelectionMenu = CreateMapSelectionMenu();
    
    int scene = MAP_SELECTION;
    int currentMap = 0;
    enum MovementType movementType = MOVE_TO;
    bool AIMode = false;
    bool AIMoveInProgess = false;
    
    sfThread *aiThread = NULL; // Thread for AI movement
    struct GameData* aiData = (struct GameData*)malloc(sizeof(struct GameData));
    aiData->bot = CreateBot();
    aiData->grid = NULL;
    aiData->step = 0;
    aiData->pathResult = NOTHING;
    aiThread = sfThread_create(MoveBot_AI, aiData);
    bool threadLaunched = false;
    
    Map maps[20];
    for (int i = 0; i < 20; i++) // Initialize all maps to empty
    {
        maps[i] = CreateMap("Empty", MAP_NULL);
    }
    
    // Define actual maps
    maps[0] = CreateMap("The Line", MAP_01);
    maps[1] = CreateMap("OBSTACLES !", MAP_02);
    maps[2] = CreateMap("The S", MAP_03);
    maps[3] = CreateMap("Make a Choice", MAP_04);
    maps[4] = CreateMap("The Maze", MAP_05);
    // Add more maps as needed here
    
    ChangeMap(mapSelectionMenu, &maps[currentMap]);
    
    /* Start the game loop */
    while (sfRenderWindow_isOpen(window))
    {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            /* Close window : exit */
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }

            // Handle input based on current scene
            switch (scene) {
            case MAP_SELECTION:
                if (event.type == sfEvtKeyPressed)
                {
                    switch (event.key.code)
                    {
                    case sfKeyRight:
                        currentMap++;
                        if (maps[currentMap].name == "Empty")
                            currentMap--;
                        ChangeMap(mapSelectionMenu, &maps[currentMap]);
                        break;
                    case sfKeyLeft:
                        currentMap--;
                        if (currentMap < 0)
                            currentMap = 0;
                        ChangeMap(mapSelectionMenu, &maps[currentMap]);
                        break;
                    case sfKeyEnter:
                        aiData->grid = CreateGrid(maps[currentMap].data);
                        SpawnBotAtStartCell(aiData->bot, aiData->grid);
                        if (AIMode)
                        {
                            printf("AI is searching for a path !\n"); // Call function for Pathfinding here (if it need to precompute path)
                            aiData->step = 0;
                        }
                        scene = GAME;
                        break;
                    case sfKeyA:
                        AIMode = !AIMode;
                        ChangeMode(mapSelectionMenu, AIMode);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case GAME:
                if (AIMode)
                {
                    if (event.type == sfEvtKeyPressed)
                    {
                        switch (event.key.code)
                        {
                        case sfKeyEnter:
                            AIMoveInProgess = !AIMoveInProgess;
                            break;
                        case sfKeyBackspace:
                            scene = MAP_SELECTION;
                            break;
                        default:
                            break;
                        }
                    }
                    if (AIMoveInProgess && scene == GAME)
                    {
                        // Search for path if not done yet
                        
                        if (!threadLaunched)
                        {
                            sfThread_launch(aiThread);
                            threadLaunched = true;
                        }
                        
                        switch(aiData->pathResult)
                        {
                        case NO_MOVE_LEFT:
                            printf("No movement left\n");
                            sfThread_terminate(aiThread);
                            scene = MAP_SELECTION;
                            break;
                        case DEAD:
                            printf("Bot is dead\n");
                            sfThread_terminate(aiThread);
                            scene = MAP_SELECTION;
                            break;
                        case REACH_END:
                            printf("Congratulations ! Bot reach the end !\n");
                            sfThread_terminate(aiThread);
                            scene = MAP_SELECTION;
                            break;
                        case NOTHING:
                        default:
                            break;
                        }
                    }
                } else
                {
                    if (event.type == sfEvtKeyPressed)
                    {
                        enum MoveResult result = NOTHING;
                        switch (event.key.code)
                        {
                        case sfKeyBackspace:
                            scene = MAP_SELECTION;
                            break;
                        case sfKeyRight:
                            result = MoveBot(aiData->bot, aiData->grid, movementType, EAST);
                            movementType = MOVE_TO;
                            break;
                        case sfKeyLeft:
                            result = MoveBot(aiData->bot, aiData->grid, movementType, WEST);
                            movementType = MOVE_TO;
                            break;
                        case sfKeyUp:
                            result = MoveBot(aiData->bot, aiData->grid, movementType, NORTH);
                            movementType = MOVE_TO;
                            break;
                        case sfKeyDown:
                            result = MoveBot(aiData->bot, aiData->grid, movementType, SOUTH);
                            movementType = MOVE_TO;
                            break;
                        case sfKeySpace:
                            movementType = JUMP;
                            break;
                        default:
                            break;
                        }
                        switch (result)
                        {
                        case DEAD:
                            printf("Unfortunately you fell of the parkour..\n");
                            scene = MAP_SELECTION;
                            break;
                        case REACH_END:
                            printf("Congratulations ! You reach the end !\n");
                            scene = MAP_SELECTION;
                            break;
                        case NOTHING:
                        case NO_MOVE_LEFT:
                            break;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }


        /* Clear the screen */
        sfRenderWindow_clear(window, sfColor_fromRGB(33, 79, 158));

        // Draw everything here
        switch (scene) {
        case MAP_SELECTION:
            DrawMapSelectionMenu(window, mapSelectionMenu);
            break;
        case GAME:
            DrawGrid(window, aiData->grid);
            DrawBot(window, aiData->bot);
            break;
        }

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    DestroyGrid(aiData->grid);
    DestroyBot(aiData->bot);
    DestroyMapSelectionMenu(mapSelectionMenu);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}
