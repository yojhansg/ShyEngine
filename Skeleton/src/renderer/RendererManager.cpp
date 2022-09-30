#include "RendererManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

RendererManager::RendererManager()
{
    window = NULL;
    screenSurface = NULL;

    initSDL();
}

bool RendererManager::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Motor TFG",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return false;
    }

    screenSurface = SDL_GetWindowSurface(window);

    if (screenSurface == NULL) {
        fprintf(stderr, "could not get window: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void RendererManager::closeSDL() {
    SDL_FreeSurface(screenSurface); screenSurface = NULL;
    SDL_DestroyWindow(window); window = NULL;
    SDL_Quit();
}

SDL_Surface* RendererManager::loadImage(std::string path)
{
    SDL_Surface* img = IMG_Load(path.c_str());
    if (img == NULL) {
        fprintf(stderr, "could not load image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Surface* optimizedImg = SDL_ConvertSurface(img, screenSurface->format, 0);
    if (optimizedImg == NULL) fprintf(stderr, "could not optimize image: %s\n", SDL_GetError());
    SDL_FreeSurface(img);
    return optimizedImg;
}