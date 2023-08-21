#pragma once

#include <iostream>
#include <vector>

union SDL_Event;
struct ImVec2;

namespace ShyEditor {

    class Window {

    protected:

        int flags;
        std::string windowName;

        int windowWidth, windowHeight;
        int windowPosX, windowPosY;

        bool focused;
        bool canBeDisplayedOnTop;
        bool docked;

        bool visible;

        virtual void Behaviour();
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

        bool IsMouseHoveringWindow();
    };
}