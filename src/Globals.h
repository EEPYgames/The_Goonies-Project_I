#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H


extern bool drawItem;
extern bool drawKey;
extern bool drawKeyLock;
extern bool doorOpen;
extern bool drawKid;
extern bool drawPotion;
extern bool drawGameKey;
extern bool attackHitbox;

extern int numKids;
extern int thelevel;
extern int timer;

// Displaying scale factor
#define GAME_SCALE_FACTOR 4.0f

// Function return values
enum class AppStatus { OK = 0, EROR, QUIT};


// Main return values
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

// Window size
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 192

// Game constants
#define MARGIN_GUI_Y 0
#define TILE_SIZE 8
#define LEVEL_WIDTH 32
#define LEVEL_HEIGHT 24

// Entities animation delay
#define ANIM_DELAY 4

#endif // GLOBALS_H