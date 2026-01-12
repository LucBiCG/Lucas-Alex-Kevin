#pragma once

#include "basics.h"

#include <SFML/System/Vector2.h>

typedef struct Cell {
    sfVector2i coord;
    enum CellType type;
    sfSprite *sprite;
} Cell;

/// <summary>
/// Loads all textures required for the cells.
/// </summary>
void LoadAllCellTextures();

/// <summary>
/// Creates and initializes a Cell structure.
/// </summary>
/// <param name="cellCoord">The coordinates of the cell in the grid.</param>
/// <param name="size">The size of the cell.</param>
/// <param name="type">The type of the cell.</param>
/// <param name="grid">The grid data for neighbor checking.</param>
/// <returns>Pointer to the newly created Cell structure.</returns>
Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[20][20]);

/// <summary>
/// Destroys the Cell structure and frees associated resources.
/// </summary>
/// <param name="cell">Pointer to the Cell structure to be destroyed.</param>
void DestroyCell(Cell* cell);

/// <summary>
/// Draws the cell on the given render window.
/// </summary>
/// <param name="window">Pointer to the SFML render window.</param>
/// <param name="cell">Pointer to the Cell structure to be drawn.</param>
void DrawCell(sfRenderWindow* window, Cell* cell);

/// <summary>
/// Determines and assigns the required sprite for the cell based on its type and neighbors.
/// </summary>
/// <param name="cell">Pointer to the Cell structure.</param>
/// <param name="grid">The grid data for neighbor checking.</param>
void GetRequiredSpriteForCell(Cell* cell, int grid[20][20]);