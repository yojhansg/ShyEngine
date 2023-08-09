#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"


namespace PEditor {

    class ScriptCreation;

    namespace ScriptCreationUtilities {

        /*
            TODO:

            Desplegable con todos los metodos
            Dividir los metodos por categorías
            -Buscador de metodos
            -Desplegables con el tipo de input
            -Mostrar cuadro de input personalizado para cada tipo
            Mostar cuadros metodos con el nombre, input con nombre y output
            Mostar lineas de union entre input y output
            Dibujar lineas de continuacion de nodos
            Hacer que se puedan tener varios nodos con el mismo nombre

            -Crear una barra de navegacion arriba para que se vea bien e incluir mas cosas
        */

        class ScriptMethod;

        class ScriptNode {

        public:

            ScriptNode();

            ScriptNode* SetID(int id);
            ScriptNode* SetPosition(int x, int y);

            void Render();

            int GetId();
            int GetY();
            int GetX();

            static ScriptNode* currentlySelected;
            void GetOutputNodePosition(float* x, float* y);

        protected:
            int id;

            float x, y;
            float w, h;

            float nodeSize;

            virtual std::string GetStringId();
            virtual void render();

            void UpdatePosition(int scrollx, int scrolly);
            void ManagerOutputNode();
        };

        class ScriptInput : public ScriptNode {

            //Definir el tipo de input, representar y guardar el valor
        public:
            ScriptInput();

            ::Components::AttributesType type;
            ::Components::AttributeValue value;

        protected:

            bool reflect;

            void render() override;
        };

        class ScriptMethod : public ScriptNode {

            ::Components::Method& method;
            std::vector<ScriptNode*> input;

        public:


            ScriptMethod(::Components::Method&);

            void render() override;
            std::string GetStringId() override;
        };


        class ScriptMenuBar {

        private:

            //Cambiar el nombre del script
            //Boton para guardar
            //Boton para cerrar
            //Desplegable con los distintos tipos de valores constantes
            //Barra de busqueda de componentes

        };

        class ScriptDropdownSelection{

        private:
            ScriptCreation* creator;

            int mousex, mousey;

        public:

            ScriptDropdownSelection(ScriptCreation* creator);
            void Render();
        };



        class Bezier {


        private:

            static float thickness;
            static int pointCount;

        public:

            static void ResetThickness();
            static void SetThickness(float t);
            static void SetPointCount(int c);
            static void ResetPointCount();


            static void Draw(int x, int y, int x1, int y1);

        };


        class Grid {

        private:
            static int spacing;
            static float thickness;
            static int interval;
            static float intervalScale;

            static int x_offset;
            static int y_offset;

            static int r, g, b, a;

        public:

            static void SetSpacing(int spacing);
            static void ResetSpacing();
            static void SetOffset(int x, int y);
            static void ResetOffset();
            static void SetInterval(int interval);
            static void ResetInterval();

            static void SetColor(int r, int g, int b, int a);
            static void ResetColor();

            static void Draw();
        };

    }
};
