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
         bool shouldOpenSaveScenePopup;

         void showRenamePopup(GameObject* gameObject);
         void showSaveScenePopup();

     public:
        MenuBar();

        virtual void render();
    };
};
