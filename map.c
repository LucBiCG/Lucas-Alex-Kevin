#include "map.h"

Map CreateMap(char* name, int mapData[20][20]) {
    Map map;
    map.name = name;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            map.data[i][j] = mapData[i][j];
        }
    }
    return map;
}