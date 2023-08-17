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
         bool shouldOpenSavePrefabPopup;

         void showRenamePopup(GameObject* gameObject);
         void showSaveScenePopup();
         void showSavePrefabPopup(GameObject* gameObject);

     public:
        MenuBar();

        virtual void render();
    };
};
