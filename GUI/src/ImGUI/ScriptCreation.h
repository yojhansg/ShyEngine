#pragma once
#include "Window.h"

class ImGUIManager;

namespace PEditor {
    class ScriptCreation : public Window
    {
    private:
        char nameBuffer[256];
        ImGUIManager* imGuiManager;


    public:
        ScriptCreation();

        virtual void render();
        void setName(std::string name);
    };
};
