#pragma once
#include "Window.h"

namespace PEditor{
    class MenuBar: public Window
    {
     private:
         float originalFramePadding;

     public:
        MenuBar();

        virtual void render();
        virtual void update();
    };
};
