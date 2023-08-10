#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"
#include "nlohmann/json_fwd.hpp"

namespace PEditor {

	class ScriptCreation;

	namespace ScriptCreationUtilities {

		/*
			TODO:

			Desplegable con todos los metodos
			Dividir los metodos por categorías
			Desplegables con el tipo de input
			Mostrar cuadro de input personalizado para cada tipo
			Mostar cuadros metodos con el nombre, input con nombre y output
			Mostar lineas de union entre input y output
			Dibujar lineas de continuacion de nodos
			Hacer que se puedan tener varios nodos con el mismo nombre
			Scroll por la escena
			Crear una barra de navegacion arriba para que se vea bien e incluir mas cosas
			Leer los managers

			-Cambiar el foreground drawList por el drawlist de la ventana
			-Buscador de metodos
			-Colorear el triangulo de salida cuando tenga al menos un input

			-Serializacion
			-Condicionales
			-Bucles
			-Implementar eventos (start, update)
			-Eliminar un nodo
			-Lista desplegable de elementos que al hacer click te lleve al elemento seleccionado

			-Establecer formato de nombres para las cosas (llamar a todo o method o function)

			-Control z manager

		*/

		class ScriptMethod;

		class ScriptNode {

		public:
			
			enum class Node {
				Method, Input, Fork
			};


			ScriptNode();

			ScriptNode* SetID(int id);
			ScriptNode* SetPosition(int x, int y);

			void Render();

			int GetId();
			int GetY();
			int GetX();

			int GetW();
			int GetH();

			static ScriptNode* currentlySelected;
			void GetOutputNodePosition(float* x, float* y);

			virtual nlohmann::json ToJson();
			virtual void FromJson(nlohmann::json&);


			Node GetType();

		protected:

			int id;
			Node type;

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
			ScriptInput(::Components::AttributesType type);

			nlohmann::json ToJson() override;

		protected:

			bool reflect; //TODO
			::Components::AttributesType attrType;
			::Components::AttributeValue attrValue;

			void render() override;

		};

		class ScriptFlow  {

		protected:

			ScriptFlow* next;
		};

		class ScriptMethod : public ScriptNode, public ScriptFlow {

			::Components::Method& method;
			std::vector<ScriptNode*> input;

		public:

			ScriptMethod(::Components::Method&);

			nlohmann::json ToJson() override;

		protected:

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
			char nameBuffer[256]; //TODO: cambiar esto a string

			bool showPopup;

			ScriptCreation* creator;

			void Close();
			void Load();
			void Save();

		public:

			ScriptMenuBar(ScriptCreation* creator);

			void SetName(const std::string& name);
			void Render();

		};

		class ScriptDropdownSelection {

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
