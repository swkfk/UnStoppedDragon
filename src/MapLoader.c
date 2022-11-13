/**
 * @file MapLoader.c
 * @author kai_Ker (kai_Ker@buaa.edu.cn)
 * @brief the source file of the map loader module
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "MapLoader.h"
#include "ConstDef.h"

#include <malloc.h>

int ML_GetBuiltinMapCount() {
    return i_builtin_map_cont;
}

void ML_LoadBuiltinMap(int *_size, int **_map, int _id) {
    int size = *_size = builtin_raw_map[_id].size;
    *_map             = (int *) malloc(sizeof(int) * size * size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (builtin_raw_map[_id].map[i * size + j] == 'X') {
                (*_map)[i * size + j] = WALL;
            } else {
                (*_map)[i * size + j] = 0;
            }
        }
    }
}

// void ML_LoadExternMap(int *, int *);

PMAP ML_MapParser(const int, const char *);