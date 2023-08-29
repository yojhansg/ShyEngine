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

         void ShowRenamePopup(Entity* entity);
         void ShowNewScenePopup();

     public:

        MenuBar();

        void Update();

    };
};
