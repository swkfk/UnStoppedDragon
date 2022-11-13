/**
 * @file MApLoader.h
 * @author kai_Ker (kai_Ker@buaa.edu.cn)
 * @brief the header file of the map loader module
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#ifndef __MAP_LOADER_H__
#define __MAP_LOADER_H__

typedef struct __MAP {
    int   size;
    int  *init_map;
    char *name;
} MAP, *PMAP;

typedef struct __RAW_MAP {
    int   size;
    char *map;
    char *name;
} RAW_MAP, *P_RAW_MAP;

extern RAW_MAP builtin_raw_map[];
extern int     i_builtin_map_cont;

int ML_GetBuiltinMapCount();

void ML_LoadBuiltinMap(int *, int **, int);
// void ML_LoadExternMap(int *, int *);

PMAP ML_MapParser(const int, const char *);

#endif
