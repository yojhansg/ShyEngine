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
			Cosas hechas: 

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
			Un nodo no se puede unir a si mismo
			Cambiar el nombre de la ventana para los inputs
			Eliminar nodo con la tecla delete
			Cambiar ids de los nodos al eliminar uno
			Al eliminar un nodo y volver a crearlo se pone en la posicion del nodo anterior
			Corregir parpadeo al crear varios nodos y eliminar uno de los anteriores
			Next node

			TODO:

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
			
			-Duplicar un nodo con control d
			-Serializar next
			-Separar grid y bezier a otro fichero
			Hacer que la curva bezier salga en la direccion del fichero
			Cambiar el nombre de fork a branch
			
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

			bool initialised; //Saber si el nodo ya ha aparecido o si es su primera vez
			bool ignoreOutput; //Marca si se quiere ignorar el boton de salida
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

		private:

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



			/*
				Se sobrescribe el id en formato string para que sea algo mas legible para el usuario
			*/
			std::string GetStringId() override;

		};

		/*
			Clase que maneja el flujo entre nodos
		*/

		class ScriptFlow  {

		private:
			
			
			int xoffset, yoffset; //Desplazamiento del punto inicial

			int flowNodeSize;	//Ancho del boton de salida
			ScriptFlow* next;   //Puntero al siguiente nodo en el flujo

			/*
				Lista de nodos anteriores a este. Util para notificar a dichos nodos
				cuando este sea eliminado
			*/
			std::vector<ScriptFlow*> previous; 

			ScriptNode* node; //Nodo asociado

		public:

			/*
				Nodo actualmente seleccionado para continuar el flujo
			*/
			static ScriptFlow* currentSelectedFlow;


			ScriptFlow(ScriptNode* node);

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
			void ManageNextNode(float x, float y, const std::string& tooltip = "");

			/*
				Obtener la posicion del boton de salida de flujo 
			*/
			void GetNextNodePosition(float* x, float* y);

			/*
				Obtener la posicion del boton de entrada de flujo
			*/
			void GetPreviousNodePosition(float* x, float* y);

			/*
				Funcionalidad para eliminar un nodo
			*/
			void OnRemoved();

			/*
				Elimina la conexion con el nodo siguiente
			*/
			void RemoveNext();

			/*
				Devuelve las dimensiones del nodo
			*/
			float GetButtonSize();

			/*
				Desplaza el punto de salida
			*/
			void SetOffset(int x, int y);

			//TODO: ver si esto merece la pena hacerlo asi o dejarlo como estaba antes
			/*
				Metodo para establecer la conexion de flujo entre dos nodos
				No elimina las conexiones existentes
			*/
			static void AddFlowConnection(ScriptFlow* previous, ScriptFlow* next);

			/*
				Metodo para establecer la conexion de flujo entre dos nodos
			*/
			static void RemoveFlowConnection(ScriptFlow* previous, ScriptFlow* next);

		};


		/*
			Nodo para representar una funcion
		*/
		class ScriptMethod : public ScriptNode {

		public:

			/*
				Para construir el nodo se debe conocer el metodo a usar
				El metodo se puede conseguir usando Components::ComponentManager
			*/
			ScriptMethod(::Components::Method&);

			/*
				Serializar el nodo con la informacion del metodo y las entradas
			*/
			nlohmann::json ToJson() override;

			/*
				Establece a una posicion del array de entradas un nodo
			*/
			void SetInput(int idx, ScriptNode* node);

			/*
				Callback para cuando un nodo es eliminado
			*/
			void OnRemoved() override;


			/*
				Busca el nodo dentro del array de entradas y lo elimina
			*/
			void RemoveInput(ScriptNode* node);

		private:

			/*
				Se guarda una referencia al contenido del metodo para evitar copias innecesarias
			*/
			::Components::Method& method;

			/*
				Vector de nodos de input
			*/
			std::vector<ScriptNode*> input;


			ScriptFlow* flow;

			/*
				Actualiza la logica y renderiza la ventana
			*/
			void updateAndRender() override;


			/*
				Sobrescribimos el id en formato string para que sea algo mas legible para el usuario (con el nombre del metodo como nombre de 
				la ventana)
			*/
			std::string GetStringId() override;
		};





		class ScriptFork: public ScriptNode {

		public: 

			enum class Fork {
				If, While, For
			};

			/*
				Para construir el nodo se debe conocer el metodo a usar
				El metodo se puede conseguir usando Components::ComponentManager
			*/
			ScriptFork(Fork type);

			/*
				Serializar el nodo con la informacion del metodo y las entradas
			*/
			nlohmann::json ToJson() override;


			/*
				Callback para cuando un nodo es eliminado
			*/
			void OnRemoved() override;


		private:

			Fork type; //El tipo de flujo del nodo
			ScriptFlow* A, * B; //Cada nodo de continuacion de flujo
			std::string a_tooltip, b_tooltip; //Informacion a mostrar al pasar el raton por cada nodo

			/*
				Actualiza la logica y renderiza la ventana
			*/
			void updateAndRender() override;


			/*
				Sobrescribimos el id en formato string para que sea algo mas legible para el usuario (con el nombre del metodo como nombre de
				la ventana)
			*/
			std::string GetStringId() override;
		};






		/*
			Clase para representar la barra de herramientas en la parte superior de la ventana

			Desde la barra se puede cambiar el nombre del script, guardar, cerrar, buscar un nodo, 
			y crear nodos de input y de fork... ect
		*/

		class ScriptMenuBar {

		private:

			//Dimension constante del array
			static const int CharBufferSize = 256;

			char nameBuffer[CharBufferSize]; //Buffer para introducir el nombre del script
			char nameSearch[CharBufferSize]; //Buffer para introducir la busqueda de un nodo

			bool showNodeSearch;	//Mostrar el desplegable con los resultados de la busqueda de nodos
			bool showClosePopup;	//Mostrar un popup cuando se intente cerrar la ventana con cambios pendientes

			ScriptCreation* creator; //Puntero al script creator para poder acceder a la lista de nodos existentes

			void Close();	//Cierra el pop up y cambia la ventana a la ventana principal del motor
			void Save();	//Serializa los nodos y los guarda en un fichero formato .script


			/*
				Muestra el desplegable con el resultado de la busqueda de nodos. Ademas procesa la creacion en caso 
				de que el usuaruo seleccione algun elemento del desplegable
			*/
			void ShowFoundMethods(std::unordered_map<std::string, Components::Component>& v, int windowW, int windowH);

			/*
				Busca un metodo dado su nombre en la lista de componentes y en la lista de managers
			*/
			Components::Method& GetMethodReference(const std::string& name);
		public:

			ScriptMenuBar(ScriptCreation* creator);
			
			/*
				Carga el fichero actual
			*/
			void Load();

			/*
				Cambia el valor del nombre del fichero
			*/
			void SetName(const std::string& name);

			/*
				Actualiza y renderiza la ventana
			*/
			void UpdateAndRender();
		};




		/*
			Desplegable con la informacion de los metodos disponibles
		*/
		class ScriptDropdownSelection {

		private:

			/*
				Puntero a creator para poder modificar la lista de nodos existentes
			*/
			ScriptCreation* creator;


			/*
				Posicion del mouse cuando se creo el desplegable. El nodo que se cree aparecera en esa posicion
			*/
			int mousex, mousey;

		public:

			ScriptDropdownSelection(ScriptCreation* creator);

			/*
				Actualiza y renderiza la ventana
			*/
			void UpdateAndRender();

			/*
				Agrega al desplegable los metodos incluidos en el mapa dado
			*/
			void AddValuesFromVector(std::unordered_map<std::string, Components::Component>& v);
		};


		/*
			Clase estatica para pintar en pantalla lineas cubicas bezier
			Representada mediante una maquina de estados para tener una funcionalidad
			parecida a la de IMGui
		*/
		class Bezier {


		private:

			//Grosor de la linea
			static float thickness;

			//Numero de puntos de la linea
			static int pointCount;

		public:

			/*
				Restablece el grosor de la linea a su valor por defecto
			*/
			static void ResetThickness();
			
			/*
				Establece el grosor de la linea a un valor en concreto
			*/
			static void SetThickness(float t);

			/*
				Establece el numero de puntos de la linea
			*/
			static void SetPointCount(int c);

			/*
				Restablece el numero de puntos a su valor determinado
			*/
			static void ResetPointCount();


			/*
				Dibuja una curva bezier cubica entre los dos puntos dados
				Los dos nodos intermedios de la curva se encuentran horizontales a los
				puntos extremos con una separacion equivalente a la distancia horizontal entre
				los dos puntos extremos

				Las lineas se dibujar por encima de todos los elementos de la pantalla
			*/
			static void Draw(int x, int y, int x1, int y1);

		};

		/*
			Clase estatica para pintar una cuadricula en el fondo de la ventana
			Representada mediante una maquina de estados para tener una funcionalidad
			parecida a la de IMGui
		*/
		class Grid {

		private:

			//Distancia entre cada una de las lineas de la cuadriculas
			static int spacing;

			//Grosor de las lineas por defecto de la cuadricula
			static float thickness;

			//A cada cuantas repeticiones hay una linea gruesa
			static int interval;

			//Escala de las lineas gruesas con respecto al grosor normal
			static float intervalScale;

			//Desplazamiento de la cuadricula
			static int x_offset, y_offset;

			//Color de las lineas
			static int r, g, b, a;

		public:

			/*
				Cambiar la distancia entre lineas	
			*/
			static void SetSpacing(int spacing);

			/*
				Restablecer la distancia entre lineas al valor por defecto
			*/
			static void ResetSpacing();

			/*
				Establecer el desplazamiento de la cuadricula
			*/
			static void SetOffset(int x, int y);

			/*
				Restablecer el desplazamiento de la cuadricula
			*/
			static void ResetOffset();

			/*
				Establecer el intervalo entre lineas gruesas
			*/
			static void SetInterval(int interval);

			/*
				Restablecer el intervalo entre lineas gruesas
			*/
			static void ResetInterval();

			/*
				Cambiar el color de las lineas
			*/
			static void SetColor(int r, int g, int b, int a);

			/*
				Cambiar el color de las lineas a su valor predeterminado
			*/
			static void ResetColor();

			/*
				Dibuja la cuadricula sobre la ventana actual
			*/
			static void Draw();
		};

	}
};
