#pragma once

#include <iostream>
#include <vector>

union SDL_Event;
struct ImVec2;

namespace ShyEditor {

    class Entity;

    struct Asset {

        char name[256];
        char extension[256];
        char path[256];
        char relativePath[256];
        bool isPrefab = false;
        int prefabId;
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

        void SetPosition(ImVec2 position);
        void SetSize(ImVec2 size);

        ImVec2 GetSize();
        ImVec2 GetPosition();

        void UpdateWindow();
        virtual void HandleInput(SDL_Event* event);

        bool IsFocused();
        bool CanBeDrawnOnTop();
        bool IsDocked();

        void Hide();
        void Show();
        bool IsVisible();


        bool IsMouseHoveringWindow();

        std::string GetWindowName();
    };
}