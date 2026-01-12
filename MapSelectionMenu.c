#include "MapSelectionMenu.h"

#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Texture.h>
#include "map.h"

static sfTexture* ARROW_TEXTURE;
static sfTexture* MANUAL_MODE_TEXTURE;
static sfTexture* AI_MODE_TEXTURE;

void LoadAllMapSelectionMenuTextures()
{
    ARROW_TEXTURE = sfTexture_createFromFile("./Assets/Arrow_R.png", NULL);
    MANUAL_MODE_TEXTURE = sfTexture_createFromFile("./Assets/Manual_M.png", NULL);
    AI_MODE_TEXTURE = sfTexture_createFromFile("./Assets/AI_M.png", NULL);
    GAME_FONT = sfFont_createFromFile("./Assets/Geo-Regular.ttf");
}

MapSelectionMenu* CreateMapSelectionMenu()
{
    LoadAllMapSelectionMenuTextures();
    
    MapSelectionMenu* mapSelectionMenu = (struct MapSelectionMenu*)malloc(sizeof(struct MapSelectionMenu));
    
    mapSelectionMenu->currentMapText = sfText_create();
    sfText_setFont(mapSelectionMenu->currentMapText, GAME_FONT);
    sfText_setCharacterSize(mapSelectionMenu->currentMapText, 50);
    sfText_setColor(mapSelectionMenu->currentMapText, sfWhite);
    sfText_setString(mapSelectionMenu->currentMapText, "Map 01");
    sfText_setOrigin(mapSelectionMenu->currentMapText, (sfVector2f){60.f, 30.f});
    sfText_setPosition(mapSelectionMenu->currentMapText, (sfVector2f){(WINDOW_WIDTH/2)-90, (WINDOW_HEIGHT/2)-5});
    
    mapSelectionMenu->rightArrowSprite = sfSprite_create();
    mapSelectionMenu->leftArrowSprite = sfSprite_create();
    sfSprite_setTexture(mapSelectionMenu->leftArrowSprite, ARROW_TEXTURE, sfTrue);
    sfSprite_setRotation(mapSelectionMenu->leftArrowSprite, 180);
    sfSprite_setTexture(mapSelectionMenu->rightArrowSprite, ARROW_TEXTURE, sfTrue);
    sfSprite_setOrigin(mapSelectionMenu->leftArrowSprite, (sfVector2f){32.f,  32.f});
    sfSprite_setOrigin(mapSelectionMenu->rightArrowSprite, (sfVector2f){32.f, 32.f});
    sfSprite_setPosition(mapSelectionMenu->rightArrowSprite, (sfVector2f){WINDOW_WIDTH - (WINDOW_WIDTH/4), WINDOW_HEIGHT/2});
    sfSprite_setPosition(mapSelectionMenu->leftArrowSprite, (sfVector2f){WINDOW_WIDTH/4, WINDOW_HEIGHT/2});
    
    mapSelectionMenu->modeIconSprite = sfSprite_create();
    sfSprite_setTexture(mapSelectionMenu->modeIconSprite, MANUAL_MODE_TEXTURE, sfTrue);
    sfSprite_setPosition(mapSelectionMenu->modeIconSprite, (sfVector2f){20, 20});
    
    mapSelectionMenu->modeText = sfText_create();
    sfText_setFont(mapSelectionMenu->modeText, GAME_FONT);
    sfText_setCharacterSize(mapSelectionMenu->modeText, 30);
    sfText_setColor(mapSelectionMenu->modeText, sfWhite);
    sfText_setString(mapSelectionMenu->modeText, "Manual Mode");
    sfText_setPosition(mapSelectionMenu->modeText, (sfVector2f){20 + 64 + 5, 20 + 16});
    
    
    return mapSelectionMenu;
}

void ChangeMode(MapSelectionMenu* mapSelectionMenu, bool aiMode)
{
    if (aiMode)
    {
        sfSprite_setTexture(mapSelectionMenu->modeIconSprite, AI_MODE_TEXTURE, sfTrue);
        sfText_setString(mapSelectionMenu->modeText, "AI Mode");
    } else
    {
        sfSprite_setTexture(mapSelectionMenu->modeIconSprite, MANUAL_MODE_TEXTURE, sfTrue);
        sfText_setString(mapSelectionMenu->modeText, "Manual Mode");
    }
}

void ChangeMap(MapSelectionMenu* mapSelectionMenu, Map* map)
{
    if (!mapSelectionMenu) return;
    
    sfText_setString(mapSelectionMenu->currentMapText, map->name);
}

void DestroyMapSelectionMenu(MapSelectionMenu* menu)
{
    if (!menu) return;
    if (menu->modeIconSprite)
        sfSprite_destroy(menu->modeIconSprite);
    if (menu->modeText)
        sfText_destroy(menu->modeText);
    if (menu->rightArrowSprite)
        sfSprite_destroy(menu->rightArrowSprite);
    if (menu->leftArrowSprite)
        sfSprite_destroy(menu->leftArrowSprite);
    if (menu->modeText)
        sfText_destroy(menu->modeText);
}

void DrawMapSelectionMenu(sfRenderWindow* window, MapSelectionMenu* menu)
{
    if (!window) return;
    if (menu->currentMapText)
        sfRenderWindow_drawText(window, menu->currentMapText, NULL);
    if (menu->rightArrowSprite)
        sfRenderWindow_drawSprite(window, menu->rightArrowSprite, NULL);
    if (menu->leftArrowSprite)
        sfRenderWindow_drawSprite(window, menu->leftArrowSprite, NULL);
    if (menu->modeIconSprite)
        sfRenderWindow_drawSprite(window, menu->modeIconSprite, NULL);
    if (menu->modeText)
        sfRenderWindow_drawText(window, menu->modeText, NULL);
}

