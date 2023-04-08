#include "RendererManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Renderer {

    RendererManager::RendererManager() : RendererManager("SDL Window", 600, 400, true) {
        //initSDL();
    }

    RendererManager::RendererManager(const std::string& title, int w, int h, bool vsync) {
        windowTitle = title;
        width = w;  height = h;
        initSDL(vsync);
    }

    RendererManager::~RendererManager() {
        closeSDL();
    }

    void RendererManager::initSDL(bool vsync) {
        // initialise SDL
        int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);
        assert(sdlInit_ret == 0);

        // Create window
        window = SDL_CreateWindow(windowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        assert(window != nullptr);

        // Create the renderer

        int vsyncEnable = vsync ? SDL_RENDERER_PRESENTVSYNC : 0;
        renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | vsyncEnable);
        assert(renderer != nullptr);

        // initialize SDL_image
        int imgInit_ret = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
        assert(imgInit_ret != 0);


        int ttfInit_ret = TTF_Init();
        assert(ttfInit_ret == 0);
    }

    void RendererManager::closeSDL() {
        SDL_DestroyWindow(window); window = NULL;
        SDL_DestroyRenderer(renderer); renderer = NULL;
        SDL_Quit();
        IMG_Quit();
    }

    SDL_Window* RendererManager::getWindow() {
        return window;
    }

    SDL_Renderer* RendererManager::getRenderer() {
        return renderer;
    }

    int RendererManager::getWidth() {
        return width;
    }

    int RendererManager::getHeight() {
        return height;
    }

    void RendererManager::toggleFullScreen() {
        auto flags = SDL_GetWindowFlags(window);

        if (flags & SDL_WINDOW_FULLSCREEN) SDL_SetWindowFullscreen(window, 0);
        else SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void RendererManager::resizeWindow(int w, int h) {
        SDL_SetWindowSize(window, w, h);
    }

    void RendererManager::renameWindow(const std::string& name) {
        SDL_SetWindowTitle(window, name.c_str());
    }

    void RendererManager::repositionWindow(int x, int y) {
        SDL_SetWindowPosition(window, x, y);
    }


    void RendererManager::showCursor(bool show) {
        SDL_ShowCursor(show);
    }

    void RendererManager::clearRenderer(cColor bg) {
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
        SDL_RenderClear(renderer);
    }

    void RendererManager::presentRenderer() {
        SDL_RenderPresent(renderer);
    }


    void RendererManager::AdjustRectToCamera(int& x, int& y, int& w, int& h) {

        auto campos = CameraPosition();
        float camscale = CameraScale();

        x += campos.x_;
        y += campos.y_;

        x *= camscale;
        y *= camscale;
        w *= camscale;
        h *= camscale;

        int winwid = Renderer::RendererManager::instance()->getWidth() * 0.5f;
        int winhei = Renderer::RendererManager::instance()->getHeight() * 0.5f;

        x += winwid - (winwid * camscale);
        y += winhei - (winhei * camscale);
    }

    Utilities::Vector2D RendererManager::CameraPosition()
    {
        return cameraPosition;
    }

    float RendererManager::CameraScale()
    {
        return cameraScale;
    }

    void RendererManager::SetCameraPosition(cVector2D vec)
    {
        cameraPosition = vec;
    }

    void RendererManager::SetCameraScale(float scale)
    {
        cameraScale = scale;
    }

}