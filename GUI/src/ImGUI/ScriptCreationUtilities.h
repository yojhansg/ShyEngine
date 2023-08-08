#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"

class ImGUIManager;

namespace PEditor {

    namespace ScriptCreationUtilities {

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

        class ScriptNode {

        public:

            ScriptNode* SetID(int id);
            ScriptNode* SetPosition(int x, int y);

            void Render();


            void Hide();
            void Show();
            bool IsHidden();


        protected:
            int id;

            float x, y;
            bool hidden;




            virtual void render();
        };

        class ScriptInput : public ScriptNode {

            //Definir el tipo de input, representar y guardar el valor
        public:

            ::Components::AttributesType type;
            //TODO: pillar el enum definido por Ivan en este caso

        protected:
            void render() override;
        };

        class ScriptMethod : public ScriptNode {

            std::string methodName;
            std::vector<std::pair<int, int>> connections;

            ScriptMethod(const std::string& methodName);
        };

        class ScriptInputHandle {


        };


        class ScriptDropdownSelection : public ScriptNode {

        protected:
            void render() override;
        };

    }
};
