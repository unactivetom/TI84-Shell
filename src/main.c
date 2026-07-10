#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timers.h>
#include <ti/screen.h>
#include <ti/getcsc.h>


#define FONT_WIDTH 13
#define FONT_HEIGHT 15
#define SCREEN_WIDTH 23
#define SCREEN_HEIGHT 16

typedef enum {
    WHITE,
    BLACK
} Color;

typedef struct {
    int x;
    int y;
    Color cursorColor;
    Color typeColor;
} Cursor;

void fillScreen(Color color);
void keyHandle(uint8_t key, Cursor *cursor);
void cursorCycle(Cursor *cursor);
void printChar(Color, char character, uint8_t x, uint8_t y, bool zeroChar);
void printString(char* string, Cursor *cursor);
void moveCursor(uint8_t distance, uint8_t direction, Cursor *cursor);


uint8_t screenBuffer[SCREEN_WIDTH][SCREEN_HEIGHT];

int main(void) {

    Cursor cursor = {0, 0, BLACK, BLACK};
    Cursor sysCursor = {0, 0, BLACK, BLACK};

    // Init systems
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
    timer_SetReload(1, 8000); // 1/4 of a second

    dbg_ClearConsole();
    os_ClrHome();

    fillScreen(BLACK);

    printString("TI-CE Shell", &sysCursor);
    sysCursor.y = 1;
    sysCursor.x = 0;
    printString("home>", &sysCursor);
    cursor.y = 1;
    cursor.x = 5;

    uint8_t key;
    while ((key = os_GetCSC()) != sk_Clear) {

        keyHandle(key, &cursor);

        // Check if timer reached 0, cycle cursor if so.
        if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
            timer_AckInterrupt(1, TIMER_RELOADED);
            cursorCycle(&cursor);
        }
    }

    // Screen buffer print only for debugging
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            dbg_printf(" 0x%02x", screenBuffer[j][i]);
        }
        dbg_printf("\n");
    }


    return 0;
}

void fillScreen(Color color) {
    for (int y = 0; y <= SCREEN_HEIGHT; ++y) {
        for (int x = 0; x <= SCREEN_WIDTH+1; ++x) {
            printChar(color, ' ', x, y, true);
        }
    }
}

void keyHandle(uint8_t key, Cursor *cursor) {
    const char *writeableChars = "\0\0\0\0\0\0\0\0\0\0\"WRMH\0\0?[VQLG\0\0:ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0";

    if (writeableChars[key]) {
        printChar(cursor->typeColor, writeableChars[key], cursor->x, cursor->y, false);
        moveCursor(1, 1, cursor);
        dbg_printf("cursor x: %d, y: %d\n", cursor->x, cursor->y);
        return;
    }
    uint8_t character = screenBuffer[cursor->x][cursor->y];
    switch (key) {
        case sk_Mode:
            printChar(cursor->typeColor, ' ', cursor->x, cursor->y, true); // Delete cursor color
            do {
                moveCursor(1, 0, cursor);
            }while (screenBuffer[cursor->x][cursor->y] == 0); // Loop till last char
            printChar(cursor->typeColor, ' ', cursor->x, cursor->y, true); // Delete last char
            dbg_printf("cursor x: %d, y: %d\n", cursor->x, cursor->y);
            break;
        case sk_Enter:
            printChar(cursor->typeColor, ' ', cursor->x, cursor->y, false); // Delete cursor color
            ++cursor->y;
            cursor->x = 0;
            dbg_printf("cursor x: %d, y: %d\n", cursor->x, cursor->y);
            break;
        case sk_Right:
            printChar(cursor->typeColor, character == 0 ? ' ' : character, cursor->x, cursor->y, character == 0);
            moveCursor(1, 1, cursor);
            break;
        case sk_Left:
            printChar(cursor->typeColor, character == 0 ? ' ' : character, cursor->x, cursor->y, character == 0);
            moveCursor(1, 0, cursor);
            break;


        default: if (key != 0)
            dbg_printf("Not a working key yet\n");
    }
}

void cursorCycle(Cursor *cursor) {
    uint8_t character = screenBuffer[cursor->x][cursor->y];
    printChar(cursor->cursorColor = cursor->cursorColor == BLACK ? WHITE : BLACK, character == 0 ? ' ' : character, cursor->x, cursor->y, character == 0);
}

void printChar(Color color, char character, uint8_t x, uint8_t y, bool zeroChar) {
    char string[2] = "";
    os_SetDrawBGColor(color == WHITE ? 65535 : 0);
    os_SetDrawFGColor(color == BLACK ? 65535 : 0);
    //dbg_printf("bg: %d, fg: %d\n", os_GetDrawBGColor(), os_GetDrawFGColor());

    sprintf(string, "%c", character);
    os_FontDrawText(string, x*FONT_WIDTH, y*FONT_HEIGHT);

    screenBuffer[x][y] = zeroChar? 0 : character;
}

void printString(char* string, Cursor *cursor) {
    for (uint8_t i = 0; i < strlen(string); ++i) {
        printChar(cursor->typeColor, string[i], cursor->x, cursor->y, string[i] == '\0');
        moveCursor(1, 1, cursor);
    }
}

void moveCursor(uint8_t distance, uint8_t direction, Cursor *cursor) {
    for (int i = 0; i < distance; ++i) {
        if (direction) {
            if (cursor->x == SCREEN_WIDTH) {++cursor->y; cursor->x = 0;}
            else ++cursor->x;
        }else {
            if (cursor->x == 0) {--cursor->y; cursor->x = SCREEN_WIDTH;}
            else --cursor->x;
        }

    }
}