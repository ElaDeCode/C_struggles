#include "definitions.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

#define BLOCK_COUNT 5

void cls(SDL_Renderer *renderer);
void handleEvents(Game *game);
void setRenderColor(SDL_Renderer *renderer, SDL_Color color);

int main() {

  Uint32 i = 0;
  Uint32 seed = time(NULL);

  Window window = {
      .width = 500,
      .height = 500,
      .logicalWidth = 100,
      .logicalHeight = 100,
      .pixelCount = window.logicalHeight * window.logicalWidth,
  };

  Blocks blocks = {
      .sand = {"sand", {200, 200, 100, 255}, 20},
      .wood = {"wood", {150, 120, 65, 255}, 60},
      .stone = {"stone", {120, 120, 120, 255}, 60},
      .obsidian = {"obsidian", {60, 30, 70, 255}, 60},
      .dirt = {"dirt", {100, 100, 50, 255}, 60},
  };
  Block *block_arr = (Block *)&blocks;

  Game game = {
      .framerate = 500,
      .shouldClose = 0,
      .isRunning = 1,
  };

  SDL_DisplayMode displayMode;
  // initialize SDL
  // ------------------------------------------------------------------

  SDL_Init(SDL_INIT_VIDEO);
  SDL_GetCurrentDisplayMode(0, &displayMode);
  window.window = SDL_CreateWindow(
      window.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      window.width, window.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  window.renderer =
      SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(window.renderer, window.logicalWidth,
                           window.logicalHeight);

  // main loop
  // ------------------------------------------------------------------------------------------------------------------------------------
  while (!game.shouldClose) {
    handleEvents(&game);

    if (game.isRunning) {
      Uint32 frameStart = SDL_GetTicks();

      // clear renderer before each frame
      cls(window.renderer);

      i++;
      seed += i / window.pixelCount;
      i = i % window.pixelCount;
      srand(seed);
      for (Uint32 j = 0; j < i; j++) {
        setRenderColor(window.renderer, block_arr[rand() % BLOCK_COUNT].color);
        SDL_RenderDrawPoint(window.renderer, j % window.logicalWidth,
                            j / window.logicalWidth);
      }
      srand(seed - 1);
      for (Uint32 j = 0; j < i; j++)
        rand();
      for (Uint32 j = i; j < window.pixelCount; j++) {
        setRenderColor(window.renderer, block_arr[rand() % BLOCK_COUNT].color);
        SDL_RenderDrawPoint(window.renderer, j % window.logicalWidth,
                            j / window.logicalWidth);
      }

      // display current buffer
      SDL_RenderPresent(window.renderer);

      // calculate delta for later use (physics)
      game.delta = SDL_GetTicks() - frameStart;
      if (game.delta < 1000 / game.framerate)
        SDL_Delay(1000 / game.framerate - game.delta);
    }
  }

  // cleanup resources
  SDL_DestroyRenderer(window.renderer);
  SDL_DestroyWindow(window.window);
  SDL_Quit();
  return 0;
}

void setRenderColor(SDL_Renderer *renderer, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// void renderCube() {
//   vec3 cubeVertex[8] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
//                         {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};
// }

void handleEvents(Game *game) {
  while (SDL_PollEvent(&game->event)) {

    switch (game->event.type) {
    case SDL_QUIT: // quit with exit button
      game->shouldClose = 1;
      break;
    case SDL_KEYDOWN:
      switch (game->event.key.keysym.sym) {
      case SDLK_RETURN:
        game->isRunning = !game->isRunning;
        break;
      case SDLK_ESCAPE: // quit with esc
        game->shouldClose = 1;
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void cls(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
}
