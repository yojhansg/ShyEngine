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
        Buscador de metodos
        Desplegables con el tipo de input
        Mostrar cuadro de input personalizado para cada tipo
        Mostar cuadros metodos con el nombre, input con nombre y output
        Mostar lineas de union entre input y output
        Dibujar lineas de continuacion de nodos
        Scroll por la escena
        Cambiar el foreground drawList por el drawlist de la ventana
    */


    class ScriptCreation : public Window
    {
    private:

        static ScriptCreation* instance;

        int xpos, ypos;
        int scrollx, scrolly;

        bool scrolled;
        bool modified;

        std::vector<ScriptCreationUtilities::ScriptNode*> nodes;

        ScriptCreationUtilities::ScriptMenuBar* menuBar;

        ImGUIManager* imGuiManager;

        ScriptCreationUtilities::ScriptDropdownSelection* dropDownSelection;

        void RenderBox(const std::string& name, ImVec2 position, ImVec2 size);


    public:
        ScriptCreation();
        ~ScriptCreation();

        void AddNode(ScriptCreationUtilities::ScriptNode* node);


        static void GetScrollPosition(int* x, int* y);
        static bool ScrolledThisFrame();

        static bool isFileModified();
        static void FileHasBeenModified();
        static void ResetModified();

        void ClearScript();
        virtual void render();

        void SetName(const std::string& name);
    };
};
