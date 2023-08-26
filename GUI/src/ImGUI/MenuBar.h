#pragma once

#include "Window.h"

class Editor;

namespace ShyEditor {

    class Entity;

    class MenuBar {

     private:

         Editor* editor;

         bool shouldOpenRenamePopup;
         bool shouldOpenNewScenePopup;
         bool shouldOpenSavePrefabPopup;

         void ShowRenamePopup(Entity* entity);
         void ShowNewScenePopup();
         void ShowSavePrefabPopup(Entity* entity);

     public:

        MenuBar();

        void Update();

    };
};
