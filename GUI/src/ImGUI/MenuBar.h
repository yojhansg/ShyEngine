#pragma once
#include "Window.h"

class ImGUIManager;

namespace PEditor{
    class MenuBar: public Window
    {
     private:
         float originalFramePadding;
         ImGUIManager* imGuiManager;

         bool shouldOpenRenamePopup;

     public:
        MenuBar();

        virtual void render();
        virtual void update();
    };
};
