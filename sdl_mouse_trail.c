#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <SDL2/SDL.h>

#define Linux

#define width 640
#define height 480

typedef struct
{
    int x;
    int y;
} iv2;

typedef struct
{
    int i;
    int length;
    iv2 *points;
} Trail;

Trail *Create_Trail(int length, int parent_x, int parent_y);
void Update_Trail(SDL_Renderer *renderer, Trail *trail, int x, int y);
void Destroy_Trail(Trail *trail);

int SDL_main(int argc, char *argv[])
{
    /*   SDL initialization   */
    printf("init start\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL is coming", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    printf("init succes\n");
    /*  init finished   */

    /*  settings    */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Event event;
    bool quit = false;

    /*int i = 0, j = 0, k = 0;*/
    int x = 0, y = 0;

    SDL_GetMouseState(&x, &y);
    Trail *mouseTrail = Create_Trail(600, x, y);
    Trail *mouseTrail_2 = Create_Trail(1200, x-10, y-10);
    /*  game main   */
    while (!quit)
    {
        SDL_WaitEventTimeout(&event,0);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_GetMouseState(&x, &y);
        Update_Trail(renderer, mouseTrail, x, y);
        Update_Trail(renderer, mouseTrail_2, x-10, y-10);

        SDL_RenderPresent(renderer);
    }
    /*game finish*/
    Destroy_Trail(mouseTrail);
    Destroy_Trail(mouseTrail_2);
    /*  cleanup   */
    printf("\n\tcleanup\n\n");
    SDL_DestroyRenderer(renderer);
    printf("renderer destroyed\n");
    SDL_DestroyWindow(window);
    printf("window destroyed\n");
    SDL_Quit();
    return 0;
}

Trail *Create_Trail(int length, int parent_x, int parent_y)
{
    static int i = 0;
    Trail *trail = malloc(sizeof(Trail) + length * sizeof(iv2));
    trail->i = 0;
    trail->length = length;
    trail->points = (iv2 *)(trail + 1);
    for (i = 0; i < length; i++)
    {
        trail->points[i].x = parent_x;
        trail->points[i].y = parent_y;
    }
    return trail;
}

void Update_Trail(SDL_Renderer *renderer, Trail *trail, int x, int y)
{
    int j, k = trail->i;
    int len = trail->length;
    iv2 *points = trail->points;
    for (int i = 0; i < len - 1; i++)
    {
        j = k;
        k = (k + 1) % len;
        SDL_SetRenderDrawColor(renderer, 100, 200, 75, 255 - (int)(255 * ((float)i / len)));
        SDL_RenderDrawLine(renderer, points[j].x, points[j].y, points[k].x, points[k].y);
    }
    points[k].x = x;
    points[k].y = y;
    trail->i = k;
}

void Destroy_Trail(Trail *trail)
{
    free(trail);
}