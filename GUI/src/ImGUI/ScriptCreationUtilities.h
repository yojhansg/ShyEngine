#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"

namespace PEditor {

    class ScriptCreation;

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

            int GetId();
            int GetY();
            int GetX();

        protected:
            int id;

            float x, y;
            float w, h;

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

            ::Components::Method& method;
            std::vector<std::pair<int, int>> connections;

        public:
            ScriptMethod(::Components::Method&);

            void render() override;
        };

        class ScriptInputHandle {


        };



        class ScriptDropdownSelection{

        private:
            ScriptCreation* creator;

            int mousex, mousey;

        public:

            ScriptDropdownSelection(ScriptCreation* creator);
            void Render();
        };

    }
};
