#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"
#include "nlohmann/json_fwd.hpp"


/*
	En este fichero se encuentran todas las clases auxiliares de ScriptCreation

	Lista de clases:
		ScriptNode
		ScriptMethod
		ScriptInput
		ScriptFlow
		ScriptFork
		ScriptDropdownSelection
		ScriptMenuBar
		Grid
		Bezier
*/



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
			Buscador de metodos
			Hacer que solo se pueda añadir una flecha si ambas salidas coinciden
			Hacer que si un nodo retorna null no se pueda añadir a otro
			Tener en cuenta que hay veces en las que hay metodos que reciben como entrada una cVariable
			Serializacion
			Lista desplegable de nodos que al hacer click te lleve al elemento seleccionado
			Guardar el output de las casillas
			Colorear el triangulo de salida cuando tenga al menos un input
			Eliminar un nodo

			-Next node
			-Condicionales
			-Bucles
			-Implementar eventos (start, update)

			-Establecer formato de nombres para las cosas (llamar a todo o method o function)

			-Cambiar el foreground drawList por el drawlist de la ventana
			-Control z manager

			-Names: Es la forma de tener metodos en el scripting
			-Comentar todo el codigo y ver que cosas se pueden mejorar

			-Mover la serializacion y carga a script creation en vez de estar en utilities

			-Cambiar la serializacion de vector2 y color a array en vez de string
			-Los nodos se pintan por encima de la barra superior
			-Cuando el input esta fuera no se dibuja la linea
			-Un nodo no se puede unir a si mismo
			-Cambiar el nombre de la ventana para los inputs
			-Eliminar nodo con la tecla delete
			-Serializar next
			-Separar grid y bezier a otro fichero
		*/

		class ScriptMethod;


		/*
			Clase que representa un nodo generico
		*/

		class ScriptNode {

		public:

			/*
				Establecer el ID para el nodo
			*/
			ScriptNode* SetID(int id);

			/*
				Devuelve el valor numerico del ID (este id es unico entre los nodos)
			*/
			int GetId();

			/*
				Devuelve un string con un ID unico para usar como nombre de la ventana
			*/
			virtual std::string GetStringId();


			/*
				Enumerado para distinguir entre los distintos tipos de nodos que puede haber
			*/
			enum class Node {
				Method, Input, Fork
			};


			/*
				Devuelve el tipo de nodo
			*/
			Node GetType();


			/*
				Cambiar la posicion del nodo
			*/
			ScriptNode* SetPosition(int x, int y);

			/*
				Actualiza y renderiza la ventana que representa al nodo
				Devuelve si el nodo va a ser eliminado durante este frame
				Solo se puede eliminar un nodo por frame
			*/
			bool UpdateAndRenderWindow();

			/*
				Devuelve la posicion x de la ventana (esquina superior izquierda)
			*/
			int GetY();

			/*
				Devuelve la posicion x de la ventana (esquina superior izquierda)
			*/
			int GetX();

			/*
				Devuelve el ancho de la ventana
			*/
			int GetW();

			/*
				Devuelve el alto de la ventana
			*/
			int GetH();

			/*
				Devuelve un string representando el tipo de valor que devuelve el nodo
			*/
			std::string GetOutputTypeString();

			/*
				Puntero al nodo que ha sido seleccionado para arrastrar su salida a otro
			*/
			static ScriptNode* currentlySelectedOutput;

			/*
				Posicion en la que colocar el boton de salida
			*/
			void GetOutputNodePosition(float* x, float* y);


			/*
				Metodo virtual para serializar el nodo a json
			*/
			virtual nlohmann::json ToJson();

			/*
				Carga los valores basicos de un nodo desde un json
			*/
			void FromJson(nlohmann::json&);

			/*
				Introduce un nuevo nodo a la lista de salidas
			*/
			void AddOutput(ScriptMethod* output);

			/*
				Elimina un nodo de la lista de salidas
			*/
			bool RemoveOutput(ScriptMethod* output);

			/*
				Callback para cuando un nodo es eliminado
			*/
			virtual void OnRemoved();

		protected:

			ScriptNode();

			int id;	//Id de la ventana
			Node type; //Tipo de nodo

			float x, y; //Posicion de la venta (Esquina superior izquierda)
			float w, h; //Dimensiones de la ventana

			bool ignoreOutput; //Marca si el 
			float outputButtonSize; //Ancho del boton de salida

			std::string outputStr; //Cadena con el nombre del tipo de salida

			/*
				Vector con las conexiones de salida del nodo. Es util para notificar a los nodos que reciben la salida
				cuando este nodo va a ser eliminado
			*/
			std::vector<ScriptMethod*> outputConexions;

			/*
				Metodo interno virtual para que cada clase que herede de nodo pueda implementar su propia logica
			*/
			virtual void updateAndRender();

			/*
				Metodo interno para actualizar la posicion de la ventana cuando esta ha sido arrastrada por el usuario
			*/
			void UpdatePositionAfterDrag(int scrollx, int scrolly);

			/*
				Logica para el boton de salida, asi como el dibujado de la flecha que une el boton de salida con el raton 
			*/
			void ManageOutputNode();

			/*
				Logica para el boton de cerrar la ventana
			*/
			bool ManageCloseNode();
		};






		/*
			Clase para representar un nodo que almacena un valor
		*/


		class ScriptInput : public ScriptNode {

		public:
			
			/*
				Para la construccion se necesita saber el tipo de entrada que se quiere almacenar en el nodo
			*/
			ScriptInput(::Components::AttributesType type);


			/*
				Serializacion del valor del nodo junto con el tipo
			*/
			nlohmann::json ToJson() override;

			/*
				Cambia el valor del nodo
			*/
			void SetValue(::Components::AttributeValue const& value);

		protected:

			bool reflect; //TODO

			/*
				Tipo de valor que se quiere guardar
			*/

			::Components::AttributesType attrType;
			/*
				Valor que se quiere guardar
			*/
			::Components::AttributeValue attrValue;


			/*
				Implementar la logica junto con el dibujado de la ventana
			*/
			void updateAndRender() override;

		};

		/*
			Clase intermedia implementar la funcionalidad relacionada con el flujo entre nodos
		*/

		class ScriptFlow: public ScriptNode  {

		public:

			/*
				Nodo actualmente seleccionado para continuar el flujo
			*/
			static ScriptFlow* currentSelectedFlow;

		protected:
			
			ScriptFlow();
			
			
			int flowNodeSize;	//Ancho del boton de salida
			ScriptFlow* next;   //Puntero al siguiente nodo en el flujo

			/*
				Lista de nodos anteriores a este. Util para notificar a dichos nodos
				cuando este sea eliminado
			*/
			std::vector<ScriptFlow*> previous; 


			/*
				Agrega un nodo a la lista de anteriores
			*/
			void AddPrevious(ScriptFlow*);

			/*
				Elimina un nodo de la lista de anteriores
			*/
			bool RemovePrevious(ScriptFlow*);

			/*
				Logica del boton de siguiente nodo
			*/
			void ManageNextNode();

			/*
				Obtener la posicion del boton de salida de flujo 
			*/
			void GetNextNodePosition(float* x, float* y);

			/*
				Obtener la posicion del boton de entrada de flujo
			*/
			void GetPreviousNodePosition(float* x, float* y);

			/*
				Callback cuando un nodo es eliminado
			*/
			void OnRemoved() override;

			/*
				Elimina la conexion con el nodo siguiente
			*/
			void RemoveNext();

			
			
			/*
				Metodo para establecer la conexion de flujo entre dos nodos
			*/
			static void AddFlowConnection(ScriptFlow* previous, ScriptFlow* next);

			/*
				Metodo para establecer la conexion de flujo entre dos nodos
			*/
			static void RemoveFlowConnection(ScriptFlow* previous, ScriptFlow* next);

		};



		class ScriptMethod : public ScriptFlow {

			::Components::Method& method;
			std::vector<ScriptNode*> input;

		public:

			ScriptMethod(::Components::Method&);

			nlohmann::json ToJson() override;

			void SetInput(int idx, ScriptNode* node);

			void OnRemoved() override;

			void OnInputRemoved(ScriptNode* node);

		protected:



			void updateAndRender() override;
			std::string GetStringId() override;
		};


		class ScriptMenuBar {

		private:

			//Cambiar el nombre del script
			//Boton para guardar
			//Boton para cerrar
			//Desplegable con los distintos tipos de valores constantes
			//Barra de busqueda de componentes

			static const int CharBufferSize = 256;

			char nameBuffer[CharBufferSize]; //TODO: cambiar esto a string
			char nameSearch[CharBufferSize]; //TODO: cambiar esto a string

			bool showNodeSearch;
			bool showPopup;

			ScriptCreation* creator;

			void Close();
			void Save();

			void AddMatchingMethods(std::unordered_map<std::string, Components::Component>& v, int windowW, int windowH);
			Components::Method& GetMethodReference(const std::string& name);
		public:
			void Load();

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

			void AddValuesFromVector(std::unordered_map<std::string, Components::Component>& v);
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
