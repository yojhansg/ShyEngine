#pragma once
#include "Window.h"
#include <string>

#include <vector>

class ImGUIManager;

namespace PEditor {



    class ScriptCreation : public Window
    {
    private:

        class ScriptNode {

        public:
            int nodeId;
            //ImVec2 position;

            virtual void Render();
        };

        class ScriptInput: public ScriptNode {

            //Definir el tipo de input, representar y guardar el valor
        };

        class ScriptMethod: public ScriptNode {

            //ImVec2 position;
            std::string methodName;
        };


        std::vector<ScriptNode*> nodes;


        char nameBuffer[256];
        ImGUIManager* imGuiManager;



        void RenderBox(const std::string& name, ImVec2 position, ImVec2 size);


    public:
        ScriptCreation();

        virtual void render();
        void setName(std::string name);
    };
};
