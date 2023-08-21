#pragma once

#include "Window.h"

class Editor;

namespace ShyEditor {

    class GameObject;

    class MenuBar {

     private:

         Editor* editor;

         bool shouldOpenRenamePopup;
         bool shouldOpenNewScenePopup;
         bool shouldOpenSavePrefabPopup;

         void showRenamePopup(GameObject* gameObject);
         void showNewScenePopup();
         void showSavePrefabPopup(GameObject* gameObject);

     public:

        MenuBar();

        void Update();

    };
};
