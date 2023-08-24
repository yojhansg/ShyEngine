#pragma once

#include <iostream>
#include <vector>

union SDL_Event;
struct ImVec2;

namespace ShyEditor {

    struct Asset {

        char name[256];
        char extension[256];
        char path[256];
        char relativePath[256];
    };

    class Window {

    protected:

        int flags;
        std::string windowName;

        int windowWidth, windowHeight;
        int windowPosX, windowPosY;

        bool focused;
        bool canBeDisplayedOnTop;
        bool docked;

        bool acceptAssetDrop;

        bool visible;

        virtual void Behaviour();
        virtual void ReceiveAssetDrop(Asset& asset);

    public:

        Window(std::string windowName, int flags);
        virtual ~Window();

        void setPosition(ImVec2 position);
        void setSize(ImVec2 size);

        ImVec2 getSize();
        ImVec2 getPosition();

        void UpdateWindow();
        virtual void HandleInput(SDL_Event* event);

        bool isFocused();
        bool CanBeDrawnOnTop();
        bool isDocked();

        void Hide();
        void Show();
        bool IsVisible();


        bool IsMouseHoveringWindow();

        std::string GetWindowName();
    };
}