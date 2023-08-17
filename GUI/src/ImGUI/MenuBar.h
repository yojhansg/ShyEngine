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
         bool shouldOpenNewScenePopup;
         bool shouldOpenSavePrefabPopup;

         void showRenamePopup(GameObject* gameObject);
         void showNewScenePopup();
         void showSavePrefabPopup(GameObject* gameObject);

     public:
        MenuBar();

        virtual void render();
    };
};
