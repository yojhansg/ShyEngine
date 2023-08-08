#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"
#include "ScriptCreationUtilities.h"

class ImGUIManager;

namespace PEditor {



    /*
    
        TODO: Cosas que hacer


        -Desplegable con todos los metodos
        -Dividir los metodos por categorías
        -Buscador de metodos
        -Desplegables con el tipo de input
        -Mostrar cuadro de input personalizado para cada tipo
        -Mostar cuadros metodos con el nombre, input con nombre y output
        -Mostar lineas de union entre input y output
        -Dibujar lineas de continuacion de nodos
    */


    class ScriptCreation : public Window
    {
    private:


        std::vector<ScriptCreationUtilities::ScriptNode*> nodes;


        char nameBuffer[256];
        ImGUIManager* imGuiManager;

        ScriptCreationUtilities::ScriptNode* selection;

        void RenderBox(const std::string& name, ImVec2 position, ImVec2 size);


    public:
        ScriptCreation();

        virtual void render();
        void setName(std::string name);
    };
};
