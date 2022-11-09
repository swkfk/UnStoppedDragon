/**
 * @file CharArray.c
 * @author kai_Ker (kai_Ker@buaa.edu.cn)
 * @brief source file of the char array (simple string in C)
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CharArray.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief create a CharArray object, filled with the given string
 *
 * @param _src
 * @return PSTRING
 */
PSTRING CA_CreateObject() {
    PSTRING pres = (PSTRING) malloc(sizeof(STRING));

    pres->string   = (char *) malloc(sizeof(char) * 5);
    pres->length   = 0;
    pres->capacity = 4;

    memset(pres->string, 0, sizeof(char) * 5);

    return pres;
}

bool CA_Dilatate(PSTRING _pString) {
    char *pre = _pString->string;
    char *new = (char *) realloc(pre, _pString->capacity / 2 * 3 + 1);

    if (new == NULL) {
        return false;
    }

    _pString->string   = new;
    _pString->capacity = _pString->capacity / 2 * 3;

    return true;
}

bool CA_AppendChar(PSTRING _pString, const char _c) {
    while (_pString->capacity < _pString->length + 1) {
        if (!CA_Dilatate(_pString)) {
            return false;
        }
    }
    _pString->string[_pString->length]     = _c;
    _pString->string[_pString->length + 1] = '\0';
    ++(_pString->length);
    return true;
}

bool CA_AppendString(PSTRING _pString, const char *_s) {
    int len = strlen(_s);
    while (_pString->capacity < _pString->length + len) {
        if (!CA_Dilatate(_pString)) {
            return false;
        }
    }
    strcat_s(_pString->string, _pString->capacity + 1, _s);
    _pString->length += len;
    return true;
}

size_t CA_GetLength(PSTRING _pString) {
    return _pString->length;
}
size_t CA_GetCapacity(PSTRING _pString) {
    return _pString->capacity;
}

void CA_Clear(PSTRING _pString) {
    _pString->string[0] = '\0';
    _pString->length    = 0;
}

void CA_Delete(PSTRING *_ppString) {
    free((*_ppString)->string);
    free(*_ppString);
    *_ppString = NULL;
}
