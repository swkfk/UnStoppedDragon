/**
 * @file UnStoppedDragon_Master.c
 * @author kai_Ker (kai_Ker@buaa.edu.cn)
 * @brief the source file of the unstopped dragon game
 * @date 2022-11-04
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * the description of the body value:
 * 000000
 * 003210
 * 004500
 * 000000
 *
 * the direction is: 1->2->3->4->5;
 * each frame, all the values will minus one, which make the tail to disappear;
 * 000000
 * 002100
 * 003400
 * 000000
 *
 * then, find the (len - 1) position, and use the direction to place the `5` properly;
 * 000000
 * 002100
 * 003450
 * 000000
 *
 * then, draw the canve, if the vaule > 0, place the snake body here;
 *
 * if eat the food, every value will plus one, before minus one
 */
#include "ConstDef.h"
#include "MapLoader.h"
#include "Utils/CharArray.h"
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NEXT(_base, _size) (((_base) + 1) % (_size))
#define PREV(_base, _size) (((_base) + -1 + (_size)) % (_size))

// CR: core function
void CR_ConsoleInit();
void CR_Init(int **, int *, int *, int);

// SC: screen & window title
void SC_Draw(int *, int, int);
void SC_Resize(int);
void SC_Clear(void);
void SC_Title(int, int);
void SC_Pause();
void SC_GameOver(int, int);
bool SC_ExitWidget(int, int);

// UD: the snake
bool UD_Move(int, int *, int, int);
bool UD_EatFood(int, int *, int, int);
void UD_SpawnFood(int *, int);
bool UD_FoodJudge(int);

int main(void) {
    // vars
    int direction, _new_direction;
    int score;
    int len;
    int score_time; // add one score every 5 frames

    // map: `0` -> empty; `FOOD(-1)` -> food; `>0` -> the body
    int *map;
    int  size;
    int  map_id;

    CR_ConsoleInit();

RESTART:
    score      = 0;
    len        = 3;
    score_time = 0;
    printf("Choose a map:\n");
    for (int i = 0; i < i_builtin_map_cont; ++i) {
        printf("%d. %s\n", i + 1, builtin_raw_map[i].name);
    }
    while (scanf_s("%d", &map_id, sizeof(map_id)), map_id < 1 || map_id > i_builtin_map_cont)
        ;
    CR_Init(&map, &size, &direction, map_id - 1);
    while (true) {
        // _kbhit() -> return true if keyboard is hit
        if (_kbhit()) {
            switch (_getch()) {
            case 'W':
            case 'w':
                _new_direction = DIRECTION_UP;
                break;
            case 'A':
            case 'a':
                _new_direction = DIRECTION_LEFT;
                break;
            case 'S':
            case 's':
                _new_direction = DIRECTION_DOWN;
                break;
            case 'D':
            case 'd':
                _new_direction = DIRECTION_RIGHT;
                break;
            case KY_NULL:
                switch (_getch()) {
                case KY_UP:
                    _new_direction = DIRECTION_UP;
                    break;
                case KY_DOWN:
                    _new_direction = DIRECTION_DOWN;
                    break;
                case KY_LEFT:
                    _new_direction = DIRECTION_LEFT;
                    break;
                case KY_RIGHT:
                    _new_direction = DIRECTION_RIGHT;
                    break;
                }
                break;
            case KY_SPACE: // press the space to pause the game
                SC_Pause();
                system("pause>nul");
                break;
            case KY_ESC: // press the ESC to exit the game
                return 0;
            }
        }

        if (_new_direction + direction && _new_direction) {
            direction = _new_direction;
        }

        ++score_time;
        if (score_time >= 5) {
            ++score;
            score_time = 0;
        }
        // judge whether the food was eaten at first
        if (UD_EatFood(direction, (int *) map, size, len)) {
            score += 5;
            ++len;
            UD_SpawnFood((int *) map, size);
        }

        // spawn the food randomly
        if (rand() % 15 == 1) {
            UD_SpawnFood((int *) map, size);
        }

        // move the snake and judge whether bump the wall or the body
        if (UD_Move(direction, (int *) map, size, len)) {
            SC_GameOver(score, len);
            if (SC_ExitWidget(score, len)) {
                system("pause>nul");
                return 0;
            } else {
                goto RESTART;
            }
        }

        // draw the canve
        SC_Draw((int *) map, size, len);

        // set the window title
        SC_Title(score, len);

        // fps = 10
        Sleep(100);
    }
    return 0;
}

/**
 * @brief core initial, the console
 *
 */
void CR_ConsoleInit() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE,
                      GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
}

/**
 * @brief initial operate
 *
 * @param map
 * @param size
 */
void CR_Init(_Out_ int **map, _Out_ int *psize, _Out_ int *direction, _In_ int map_id) {
    while (_kbhit()) {
        _getch();
    }

    *direction = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
    ML_LoadBuiltinMap(psize, map, map_id);
    ((int(*)[*psize])(*map))[*psize / 2][*psize / 2 - 1] = (*direction == DIRECTION_RIGHT ? 1 : 3);
    ((int(*)[*psize])(*map))[*psize / 2][*psize / 2]     = 2;
    ((int(*)[*psize])(*map))[*psize / 2][*psize / 2 + 1] = (*direction == DIRECTION_RIGHT ? 3 : 1);

    SC_Resize(*psize);
    SC_Clear();
    srand((unsigned int) time(NULL));
    for (int i = 0; i < 3; ++i) {
        UD_SpawnFood(*map, *psize);
    }
}

/**
 * @brief draw the game board
 *
 * @param map
 * @param size
 */
void SC_Draw(int *map, int size, int len) {
    PSTRING pDisplay = CA_CreateObject();
    int(*pArr)[size] = (int(*)[size]) map;
    int val;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            val = (*(pArr + i))[j];
            if (val == len) {
                CA_AppendString(pDisplay, "\033[36m{}\033[0m");
            } else if (val > 0) {
                CA_AppendString(pDisplay, "\033[32m()\033[0m");
            } else if (val == WALL) {
                CA_AppendString(pDisplay, "\033[41;37mXX\033[0m");
            } else if (UD_FoodJudge(val)) {
                CA_AppendString(pDisplay, "\033[33m$");
                CA_AppendChar(pDisplay, '1' + (-val) / 10);
                CA_AppendString(pDisplay, "\033[0m");
            } else {
                CA_AppendString(pDisplay, "  ");
            }
        }
        if (i < size - 1)
            CA_AppendChar(pDisplay, '\n');
    }
    SC_Clear();
    fputs(pDisplay->string, stdout);
    CA_Delete(&pDisplay);
}

/**
 * @brief resize the concole size
 *
 * @param size
 */
void SC_Resize(int size) {
    char cmd[50];
    sprintf(cmd, "mode con cols=%d lines=%d", size * 2, size);
    system(cmd);
}

/**
 * @brief clear the console
 *
 */
void SC_Clear() {
    system("cls");
}

/**
 * @brief set the window title on normal mode
 *
 * @param score
 * @param len
 */
void SC_Title(int score, int len) {
    char cmd[100];
    sprintf(cmd, "title Score: %d; Length: %d", score, len);
    system(cmd);
}

/**
 * @brief set the window title on pause mode
 *
 */
void SC_Pause() {
    system("title ----paused----");
}

/**
 * @brief set the window title after gameover
 *
 * @param score
 * @param len
 */
void SC_GameOver(int score, int len) {
    char cmd[100];
    sprintf(cmd, "title GameOver! Score: %d; Length: %d", score, len);
    system(cmd);
}

/**
 * @brief display a popup window to ask whether to restart
 *
 * @param score
 * @param len
 * @return true restart
 * @return false exit
 */
bool SC_ExitWidget(int score, int len) {
    char content[100];
    sprintf(content, "Score: %d; Length: %d\nWhether to restart?", score, len);
    // LPTSTR title = (LPTSTR) malloc(BUFSIZ);
    // GetConsoleTitle(title, BUFSIZ);
    return !(MessageBox(GetConsoleWindow(), content, "GameOver!", MB_YESNO) == IDYES);
}

/**
 * @brief move the snake
 *
 * @param direction the direction to go
 * @param map
 * @param size
 * @param len the length of the snake
 * @return true the snake bump the wall or its body -> gameover
 * @return false nothing bad happened
 */
bool UD_Move(int direction, int *map, int size, int len) {
    int(*pArr)[size] = (int(*)[size]) map;
    bool bump        = false;

    // the body value minus one
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((*(pArr + i))[j] >= 1) {
                (*(pArr + i))[j]--;
            }
            if ((*(pArr + i))[j] >= FOOD && (*(pArr + i))[j] < 0) {
                (*(pArr + i))[j]++;
            }
        }
    }

    // check bump
    int *tgt;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((*(pArr + i))[j] == len - 1) {
                switch (direction) {
                case DIRECTION_UP:
                    tgt = &(*(pArr + PREV(i, size)))[j];
                    break;
                case DIRECTION_DOWN:
                    tgt = &(*(pArr + NEXT(i, size)))[j];
                    break;
                case DIRECTION_LEFT:
                    tgt = &(*(pArr + i))[PREV(j, size)];
                    break;
                case DIRECTION_RIGHT:
                    tgt = &(*(pArr + i))[NEXT(j, size)];
                    break;
                }
                if (*tgt > 0 || *tgt == WALL) {
                    bump = true;
                } else {
                    *tgt = len;
                }
            }
        }
    }

    return bump;
}

/**
 * @brief check whether the next step is food
 *
 * @param direction
 * @param map
 * @param size
 * @param len the length of the snake
 * @return true will eat the food
 * @return false the next step is the wall, its body or empty
 */
bool UD_EatFood(int direction, int *map, int size, int len) {
    int(*pArr)[size] = (int(*)[size]) map;
    bool eat_food    = false;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((*(pArr + i))[j] == len) {
                switch (direction) {
                case DIRECTION_UP:
                    if (i > 0 && UD_FoodJudge((*(pArr + i - 1))[j])) {
                        eat_food = true;
                    }
                    break;
                case DIRECTION_DOWN:
                    if (i < size - 1 && UD_FoodJudge((*(pArr + i + 1))[j])) {
                        eat_food = true;
                    }
                    break;
                case DIRECTION_LEFT:
                    if (j > 0 && UD_FoodJudge((*(pArr + i))[j - 1])) {
                        eat_food = true;
                    }
                    break;
                case DIRECTION_RIGHT:
                    if (j < size - 1 && UD_FoodJudge((*(pArr + i))[j + 1])) {
                        eat_food = true;
                    }
                    break;
                }
            }
        }
    }

    // if will eat the food, every body value will plus one
    if (eat_food) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((*(pArr + i))[j] > 0) {
                    ++(*(pArr + i))[j];
                }
            }
        }
    }
    return eat_food;
}

/**
 * @brief spawn the food on the *empty* place, will retry for 100 times
 *
 * @param map
 * @param size
 */
void UD_SpawnFood(int *map, int size) {
    int(*pArr)[size] = (int(*)[size]) map;
    for (int _retry = 0; _retry < 100; ++_retry) {
        int x = rand() % size;
        int y = rand() % size;
        if ((*(pArr + x))[y] == 0) {
            (*(pArr + x))[y] = FOOD + rand() % 20 + 200;
            return;
        }
    }
}

/**
 * @brief judge whether a value is a food
 *
 * @param val
 * @return true
 * @return false
 */
bool UD_FoodJudge(int val) {
    return val >= FOOD && val < 0;
}
