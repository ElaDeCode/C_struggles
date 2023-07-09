#pragma once
#include <SDL2/SDL.h>

typedef struct {
  float x, y, z;
} vec3;

typedef struct {
  char name[10];
  SDL_Color color;
  float mass;
} Block;

typedef struct {
  char title[30];
  Uint16 width, height, logicalWidth, logicalHeight;
  Uint32 pixelCount;
  SDL_Window *window;
  SDL_Renderer *renderer;
} Window;

typedef struct {
  Block sand, wood, stone, obsidian, dirt;
  Uint8 count;
} Blocks;

typedef struct {
  unsigned int framerate;
  SDL_bool isRunning, shouldClose;
  SDL_Event event;
  Uint32 delta;
} Game;
