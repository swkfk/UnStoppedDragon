/**
 * @file CharArray.h
 * @author kai_Ker (kai_Ker@buaa.edu.cn)
 * @brief header file of the char array (simple string in C)
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stddef.h>
#ifndef __CHAR_ARRAY_H__
#define __CHAR_ARRAY_H__

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct __STRING {
    char  *string;
    size_t capacity; // sizeof(string) == capacity + 1
    size_t length;
} STRING, *PSTRING;

PSTRING CA_CreateObject();
bool    CA_Dilatate(PSTRING);

bool CA_AppendChar(PSTRING, const char);
bool CA_AppendString(PSTRING, const char *);

size_t CA_GetLength(PSTRING);
size_t CA_GetCapacity(PSTRING);

void CA_Clear(PSTRING);
void CA_Delete(PSTRING *);

#endif
