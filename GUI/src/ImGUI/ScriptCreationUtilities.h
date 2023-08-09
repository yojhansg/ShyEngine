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
            -Hacer que se puedan tener varios nodos con el mismo nombre
        */

        class ScriptMethod;

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

            void SetOutput(ScriptMethod* node);
            ScriptMethod* GetOutput();

            static ScriptNode* currentlySelected;

        protected:
            int id;

            float x, y;
            float w, h;

            bool hidden;
            ScriptMethod* output;

            virtual std::string GetStringId();
            virtual void render();

            void UpdatePosition(int scrollx, int scrolly);
            void ManagerOutputNode();
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
            std::vector<ScriptNode*> input;

        public:


            ScriptMethod(::Components::Method&);

            void render() override;
            std::string GetStringId() override;
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
