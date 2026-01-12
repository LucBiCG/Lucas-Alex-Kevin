#include "Cell.h"

#include <SFML/Graphics.h>
#include <SFML/Graphics/Texture.h>

static sfTexture* EMPTY_CELL_TEXTURE = NULL;
static sfTexture* EMPTY_NORTH_SOUTH_CELL_TEXTURE = NULL;
static sfTexture* EMPTY_EAST_WEST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_NORTH_SOUTH_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_EAST_WEST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_NORTH_EAST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_NORTH_WEST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_SOUTH_EAST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_SOUTH_WEST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_T_JUNCTION_NORTH_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_T_JUNCTION_SOUTH_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_T_JUNCTION_EAST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_T_JUNCTION_WEST_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_CROSSROAD_CELL_TEXTURE = NULL;
static sfTexture* WALKABLE_CELL_TEXTURE = NULL;
static sfTexture* START_CELL_TEXTURE = NULL;
static sfTexture* END_CELL_TEXTURE = NULL;
static sfTexture* OBSTACLE_NORTH_SOUTH_CELL_TEXTURE = NULL;
static sfTexture* OBSTACLE_EAST_WEST_CELL_TEXTURE = NULL;

void LoadAllCellTextures() {
    EMPTY_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Empty.png", NULL);
    EMPTY_NORTH_SOUTH_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Empty_North_South.png", NULL);
    EMPTY_EAST_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Empty_East_West.png", NULL);
    WALKABLE_NORTH_SOUTH_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_North_South.png", NULL);
    WALKABLE_EAST_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_East_West.png", NULL);
    WALKABLE_NORTH_EAST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_North_East.png", NULL);
    WALKABLE_NORTH_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_North_West.png", NULL);
    WALKABLE_SOUTH_EAST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_South_East.png", NULL);
    WALKABLE_SOUTH_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_South_West.png", NULL);
    WALKABLE_T_JUNCTION_NORTH_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_T_Junction_North.png", NULL);
    WALKABLE_T_JUNCTION_SOUTH_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_T_Junction_South.png", NULL);
    WALKABLE_T_JUNCTION_EAST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_T_Junction_East.png", NULL);
    WALKABLE_T_JUNCTION_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_T_Junction_West.png", NULL);
    WALKABLE_CROSSROAD_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable_Crossroad.png", NULL);
    WALKABLE_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Walkable.png", NULL);
    START_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Start.png", NULL);
    END_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/End.png", NULL);
    OBSTACLE_NORTH_SOUTH_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Obstacle_North_South.png", NULL);
    OBSTACLE_EAST_WEST_CELL_TEXTURE = sfTexture_createFromFile("./Assets/Tiles/Style A/Obstacle_East_West.png", NULL);
    
    printf("Texture Loaded !\n");
}

Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[20][20]) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (!cell) {
        return NULL;
    }
    
    cell->coord = cellCoord;
    
    cell->type = type;
    
    cell->sprite = sfSprite_create();
    GetRequiredSpriteForCell(cell, grid);
    sfSprite_setPosition(cell->sprite, (sfVector2f){(float)cellCoord.x * size, (float)cellCoord.y * size});
    float scale = (float)CELL_SIZE / 8.f;
    sfSprite_setScale(cell->sprite, (sfVector2f){scale, scale});
    
    return cell;
}

void DestroyCell(Cell* cell) {
    if (cell) {
        if (cell->sprite) {
            sfSprite_destroy(cell->sprite);
        }
        free(cell);
    }
}

void DrawCell(sfRenderWindow* window, Cell* cell) {
    if (window && cell && cell->sprite) {
        sfRenderWindow_drawSprite(window, cell->sprite, NULL);
    }
}

void GetRequiredSpriteForCell(Cell* cell, int grid[20][20]) {
    // This function would determine the correct sprite for the cell based on its type and possibly its neighbors
    // For simplicity, this function is left unimplemented
    sfTexture* texture = NULL;
    
    int neighbours = 0;
    if (cell->type != EMPTY)
    {
        if (cell->coord.x - 1 >= 0)
        {
            if (grid[cell->coord.y][cell->coord.x - 1] != EMPTY) {
                neighbours += 1;
            }
        }
        if (cell->coord.x + 1 < GRID_ROWS)
        {
            if (grid[cell->coord.y][cell->coord.x + 1] != EMPTY) {
                neighbours += 2;
            }
        }
        if (cell->coord.y - 1 >= 0) 
        {
            if (grid[cell->coord.y - 1][cell->coord.x] != EMPTY) {
                neighbours += 10;
            } 
        }
        if (cell->coord.y + 1 < GRID_ROWS)
        {
            if (grid[cell->coord.y + 1][cell->coord.x] != EMPTY) {
                neighbours += 20;
            }
        }
    }
    
    switch (cell->type)
    {
        case EMPTY:
            // Load or assign sprite for EMPTY
            texture = EMPTY_CELL_TEXTURE;
            break;
    case WALKABLE:
            // Load or assign sprite for WALKABLE
            switch (neighbours)
            {
            case NONE:
                texture = WALKABLE_CELL_TEXTURE;
                break;
            case EAST:
            case WEST:
            case EAST_WEST:
                    texture = WALKABLE_EAST_WEST_CELL_TEXTURE;
                    break;
            case SOUTH:
            case NORTH:
            case NORTH_SOUTH:
                    texture = WALKABLE_NORTH_SOUTH_CELL_TEXTURE;
                    break;
            case SOUTH_EAST:
                texture = WALKABLE_SOUTH_EAST_CELL_TEXTURE;
                break;
            case SOUTH_WEST:
                texture = WALKABLE_SOUTH_WEST_CELL_TEXTURE;
                break;
            case NORTH_EAST:
                texture = WALKABLE_NORTH_EAST_CELL_TEXTURE;
                break;
            case NORTH_WEST:
                texture = WALKABLE_NORTH_WEST_CELL_TEXTURE;
                break;
            case NORTH_EAST_WEST:
                texture = WALKABLE_T_JUNCTION_NORTH_CELL_TEXTURE;
                break;
            case SOUTH_EAST_WEST:
                texture = WALKABLE_T_JUNCTION_SOUTH_CELL_TEXTURE;
                break;
            case NORTH_SOUTH_EAST:
                texture = WALKABLE_T_JUNCTION_EAST_CELL_TEXTURE;
                break;
            case NORTH_SOUTH_WEST:
                texture = WALKABLE_T_JUNCTION_WEST_CELL_TEXTURE;
                break;
            case ALL:
                texture = WALKABLE_CROSSROAD_CELL_TEXTURE;
                break;
            default:
                break;
            }
            break;
        case START:
            // Load or assign sprite for START
            texture = START_CELL_TEXTURE;
            break;
        case END:
            // Load or assign sprite for END
            texture = END_CELL_TEXTURE;
            break;
        case OBSTACLE:
            // Load or assign sprite for OBSTACLE
            switch (neighbours)
            {
                case NONE:
                case EAST:
                case WEST:
                case NORTH:
                case SOUTH:
                case SOUTH_EAST:
                case SOUTH_WEST:
                case NORTH_EAST:
                case NORTH_WEST:
                default:
                    texture = EMPTY_CELL_TEXTURE;
                    break;
                case EAST_WEST:
                    texture = OBSTACLE_EAST_WEST_CELL_TEXTURE;
                    break;
                case NORTH_SOUTH:
                    texture = OBSTACLE_NORTH_SOUTH_CELL_TEXTURE;
                    break;
            }
            break;
        default:
            break;        
    }
    if (texture) {
        sfSprite_setTexture(cell->sprite, texture, sfTrue);
    }
}