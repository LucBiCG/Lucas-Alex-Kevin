#pragma once

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include "basics.h"
#include "grid.h"

struct Map;

/// <summary>
/// Structure to represent a movement command for the Bot.
/// </summary>
/// <remarks>
/// This structure contains the type of movement and the direction.
/// </remarks>
struct Move
{
    enum MovementType type;
    enum Direction direction;
};

/// <summary>
/// Structure to represent a Bot in the game.
/// </summary>
/// <remarks>
/// This structure contains the bot's position, sprite, and movement queue.
/// </remarks>
struct Bot
{
    sfVector2i position;
    sfSprite *sprite;
    struct Move MoveQueue[];
};

/// <summary>
/// Structure to hold game data for AI movement.
/// </summary>
/// <remarks>
/// This structure contains pointers to the bot and grid,
/// as well as variables to track the current step and path result.
/// </remarks>
struct GameData
{
    struct Bot* bot;
    Grid* grid;
    int step;
    int pathResult;
};

/// <summary>
/// Creates and initializes a Bot structure.
/// </summary>
/// <returns>Pointer to the newly created Bot structure.</returns>
struct Bot* CreateBot();

/// <summary>
/// Spawns the bot at the start cell of the grid.
/// </summary>
/// <param name="bot">Pointer to the Bot structure.</param>
/// <param name="grid">Pointer to the Grid structure.</param>
void SpawnBotAtStartCell(struct Bot* bot, Grid* grid);

/// <summary>
/// Destroys the Bot structure and frees associated resources.
/// </summary> 
/// <param name="bot">Pointer to the Bot structure to be destroyed.</param>
void DestroyBot(struct Bot* bot);

/// <summary>
/// Draws the bot on the given render window.
/// </summary>
/// <param name="window">Pointer to the SFML render window.</param>
/// <param name="bot">Pointer to the Bot structure to be drawn.</param>
void DrawBot(sfRenderWindow* window, struct Bot* bot);

/// <summary>
/// Moves the bot in the specified direction and movement type.
/// </summary>
/// <param name="bot">Pointer to the Bot structure.</param>
/// <param name="grid">Pointer to the Grid structure.</param>
/// <param name="type">Type of movement (MOVE_TO or JUMP).</param>
/// <param name="direction">Direction of movement (NORTH, EAST, SOUTH, WEST).</param>
/// <returns>Result of the movement (e.g., SUCCESS, DEAD, REACH_END).</returns>
int MoveBot(struct Bot* bot, Grid* grid, enum MovementType type, enum Direction direction);

/// <summary>
/// Adds a movement command to the bot's movement queue.
/// </summary>
/// <param name="bot">Pointer to the Bot structure.</param>
/// <param name="type">Type of movement (MOVE_TO or JUMP).</param>
/// <param name="direction">Direction of movement (NORTH, EAST, SOUTH, WEST).</param>
void AddMovement(struct Bot* bot, enum MovementType type, enum Direction direction);

/// <summary>
/// Thread function to move the bot using AI.
/// </summary>
/// <param name="data">Pointer to the GameData structure containing bot and grid information.</param>
void MoveBot_AI(struct GameData* data);

/// <summary>
/// Searches for a path for the bot using AI algorithms.
/// </summary>
/// <param name="bot">Pointer to the Bot structure.</param>
/// <param name="grid">Pointer to the Grid structure.</param>
/// <returns>True if a path is found, false otherwise.</returns>
bool SearchPath_AI(struct Bot* bot, Grid* grid);