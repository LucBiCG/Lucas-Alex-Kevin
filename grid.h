#pragma once

#include <SFML/System.h>

#include "basics.h"
#include "cell.h"

typedef struct Grid
{
    Cell* cell[20][20];
} Grid;

/// <summary>
/// Creates and initializes a Grid structure based on the provided grid data.
/// </summary>
/// <param name="gridData">A 2D array representing the grid data.</param>
/// <returns>Pointer to the newly created Grid structure.</returns>
Grid* CreateGrid(int gridData[20][20]);

/// <summary>
/// Destroys the Grid structure and frees associated resources.
/// </summary>
/// <param name="grid">Pointer to the Grid structure to be destroyed.</param>
void DestroyGrid(Grid* grid);

/// <summary>
/// Draws the grid on the given render window.
/// </summary>
/// <param name="window">Pointer to the SFML render window.</param>
/// <param name="grid">Pointer to the Grid structure to be drawn.</param>
void DrawGrid(sfRenderWindow* window, Grid* grid);
