#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics/Types.h>

#define WINDOW_TITLE "LiteBot"
#define WINDOW_WIDTH 800    // CELL_SIZE * GRID_COLS
#define WINDOW_HEIGHT 800   // CELL_SIZE * GRID_ROWS

#define GRID_ROWS 20
#define GRID_COLS 20
#define CELL_SIZE 40
#define GRAPHIC_OFFSET 20

static sfFont* GAME_FONT;

/// <summary>
/// Use this enum to define some exit codes for the program (just for debug purpose)
/// </summary>
enum ExitCode {
    FAILURE = -1,
    SUCCESS = 1,
    NULL_TEXT = 60,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
};

/// <summary>
/// Use this enum to define the different scenes of the game
/// </summary>
enum Scene {
    MAP_SELECTION,
    GAME,
};

/// <summary>
/// Use this enum to define the different types of cells in the grid
/// </summary>
enum CellType {
    EMPTY,
    WALKABLE,
    START,
    END,
    OBSTACLE,
};

/// <summary>
/// Use this enum to define the different directions for movement and cell connections
/// </summary>
enum Direction
{
    NONE = 0,
    EAST = 1,
    WEST = 2,
    EAST_WEST = 3,
    NORTH = 10,
    NORTH_EAST = 11,
    NORTH_WEST = 12,
    NORTH_EAST_WEST = 13,
    SOUTH = 20,
    SOUTH_EAST = 21,
    SOUTH_WEST = 22,
    SOUTH_EAST_WEST = 23,
    NORTH_SOUTH = 30,
    NORTH_SOUTH_EAST = 31,
    NORTH_SOUTH_WEST = 32,
    ALL = 33,
};

/// <summary>
/// Use this enum to define the different types of movement for the bot
/// </summary>
enum MovementType
{
    MOVE_TO,
    JUMP,
};

/// <summary>
/// Use this enum to define the different results of a bot movement
/// </summary>
enum MoveResult
{
    NO_MOVE_LEFT = -2,
    DEAD = -1,
    NOTHING = 0,
    REACH_END = 1,
};