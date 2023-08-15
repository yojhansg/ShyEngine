#pragma once
#include "Window.h"

class ImGUIManager;

namespace PEditor{
    class GameObject;

    class MenuBar: public Window
    {
     private:
         ImGUIManager* imGuiManager;

         bool shouldOpenRenamePopup;

         void showRenamePopup(GameObject* gameObject);

     public:
        MenuBar();

        virtual void render();
    };
};
