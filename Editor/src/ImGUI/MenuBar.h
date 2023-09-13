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

         float playOptionsWidth;

         void ShowRenamePopup(Entity* entity);
         void ShowNewScenePopup();

         void Debug();
         void Release();
         void Stop();

     public:

        MenuBar();

        void Update();

    };
};
