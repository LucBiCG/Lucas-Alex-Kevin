#pragma once

#include "basics.h"
#include "map.h"

typedef struct MapSelectionMenu
{
    sfText* currentMapText;
    sfSprite* leftArrowSprite;
    sfSprite* rightArrowSprite;
    sfSprite* modeIconSprite;
    sfText* modeText;
} MapSelectionMenu;

/// <summary>
/// Loads all textures required for the map selection menu.
/// </summary>
void LoadAllMapSelectionMenuTextures();

/// <summary>
/// Creates and initializes a MapSelectionMenu structure.
/// </summary>
/// <returns>Pointer to the newly created MapSelectionMenu structure.</returns>
MapSelectionMenu* CreateMapSelectionMenu();

/// <summary>
/// Changes the mode icon and text in the map selection menu.
/// </summary>
/// <param name="mapSelectionMenu">Pointer to the MapSelectionMenu structure.</param>
/// <param name="aiMode">Boolean indicating whether AI mode is enabled.</param>
void ChangeMode(MapSelectionMenu* mapSelectionMenu, bool aiMode);

/// <summary>
/// Changes the displayed map in the map selection menu.
/// </summary>
/// <param name="mapSelectionMenu">Pointer to the MapSelectionMenu structure.</param>
/// <param name="map">Pointer to the Map structure to be displayed.</param>
void ChangeMap(MapSelectionMenu* mapSelectionMenu, Map* map);

/// <summary>
/// Destroys the MapSelectionMenu structure and frees associated resources.
/// </summary>
/// <param name="menu">Pointer to the MapSelectionMenu structure to be destroyed.</param
void DestroyMapSelectionMenu(MapSelectionMenu* menu);

/// <summary>
/// Draws the map selection menu on the given render window.
/// </summary>
/// <param name="window">Pointer to the SFML render window.</param>
/// <param name="menu">Pointer to the MapSelectionMenu structure to be drawn.</param
void DrawMapSelectionMenu(sfRenderWindow* window, MapSelectionMenu* menu);