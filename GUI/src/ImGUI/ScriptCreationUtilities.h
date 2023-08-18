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
			Condicionales
			Bucles
			Recibir condicion flujo
			Cambiar la posicion de entrada del flujo
			Hacer que la curva bezier salga en la direccion del fichero
			Implementar eventos (start, update)
			Mover la serializacion y carga a script creation en vez de estar en utilities

			No mostrar el pop up cuando no haya cambios pendientes
			Hacer que no se pueda crear dos eventos iguales
			Oscurecer la pantalla cuando aparezca el popup de cerrar
			Añadir soporte para comentarios
			Serializar next
			Serializar condicionales
			Serializar eventos
			Serializar comentarios
			SerializedValues: Es la forma de serializar variables en el scripting

			TODO:

			-En la escena, renderizar el rectangulo de la camara lo ultimo para evitar que cambie de grosor
			-Resource manager editor
			-Motor: Guardar los valores serializados para cada script, no 
			para cada entidad
			-Color de la escena
			-Highlight color

			-La creacion de un nodo no tiene en cuenta el scroll
			-Hacer que al pulsar el boton de añadir un script añada el script una vez se cierre la ventana de scripting
			-Cuando se edite un script que ya ha sido creado no permitir cambiar el nombre
			-Quitar try catchs del codigo
			Quitar la edicion de nombre del script. En el momento en el que se guarde pedir el nombre al usuario

			===Funcionalidad nueva===
			-Si no se seleccion la primera entidad entonces que se seleccione la entidad del script
			-Poder serializar entidades
			-Names: Es la forma de tener metodos en el scripting

			Para el motor: falta el setSerializedValue
			Falta terminar las colecciones (arrays)

			-Duplicar un nodo con control d
			-Control z manager
			-Poder quitar un nodo siguiente
			-Todos los metodos de componentes tienen como primer parametro un puntero a la entidad

			-Quitar el scroll de la escena en el caso de no tener el focus en la ventana (o que haya otra ventana por delante)
			-Cambiar como se importan los assets de imagenes
			-Hacer que al crear una imagen por defecto tenga de escala el tamaño de la imagen
			-Cambiar el punto centra de la escena por la imagen

			===Mejoras de codigo e implementacion===

			-Separar Utilities en varios ficheros
			-Establecer formato de nombres para las cosas (llamar a todo o method o function)
			-Comentar todo el codigo y ver que cosas se pueden mejorar
			-Cambiar la serializacion de vector2 y color a array en vez de string
			-Crear un metodo para la creacion centrada de nodos para evitar codigo repetido
			-Cambiar las llamadas de windowpos y windowsize por 'x y w z'
			-Simplificar el uso del flow a un unico metodo y que los parametros sean propiedades
			-Hacer que el id no tenga en cuenta el tamaño del array ya que puede haber elementos sin id
			-Establecer correctamente los tamaños de los nodos para que no queden feotes
			-En algun momento cambiar la serializacion de los eventos en el motor
			-Empezar sin cambios aunque esten los dos nodos creados
			-Establecer los cambios cuando se añada input y todas esas cosas

			===Correccion de errores===

			-Los nodos se pintan por encima de la barra superior
			-Cuando el input esta fuera no se dibuja la linea
			-Cuando el next esta fuera no se dibuja la linea
			-Al retraer una ventana hacer que las flechas apunten a la posicion correcta
			-Ordenar la lista de metodos y esas cosas
			-Unificar todos los tipos de entradas string
			-Verificar el tipo de la condicion para los condicionales (if, while)
			-Cuando hay dos inputs iguales y uno se borra ocurre un error
			-Spawnear comentarios en el centro de la pantalla


			
		*/

		class ScriptFlow;
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
				Enumerado para distinguir entre los distintos tipos de nodos que puede haber
			*/
			enum class Node {
				Method, Input, Fork, Event, Comment
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
				Introduce un nuevo nodo a la lista de salidas
			*/
			void AddOutput(ScriptNode* output);

			/*
				Elimina un nodo de la lista de salidas
			*/
			bool RemoveOutput(ScriptNode* output);


			/*
				Carga los valores basicos de un nodo desde un json
			*/
			void FromJson(nlohmann::json&);



		protected:

			ScriptNode();

			int id;	//Id de la ventana
			Node type; //Tipo de nodo

			float x, y; //Posicion de la venta (Esquina superior izquierda)
			float w, h; //Dimensiones de la ventana

			bool initialised; //Saber si el nodo ya ha aparecido o si es su primera vez
			bool ignoreOutput; //Marca si se quiere ignorar el boton de salida
			float outputButtonSize; //Ancho del boton de salida

			bool resizable; //Se pueden cambiar las dimensiones de la ventana

			std::string outputStr; //Cadena con el nombre del tipo de salida

			/*
				Vector con las conexiones de salida del nodo. Es util para notificar a los nodos que reciben la salida
				cuando este nodo va a ser eliminado
			*/
			std::vector<ScriptNode*> outputConexions;

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




			//==================== Virtual functionality =================================
			/*
				Metodo interno virtual para que cada clase que herede de nodo pueda implementar su propia logica
			*/
			virtual void updateAndRender();

		public:

			/*
				Callback para cuando un nodo es eliminado
			*/
			virtual void OnRemoved();

			/*
				Cuando una de los nodos entradas ha sido eliminado
			*/
			virtual void RemoveInput(ScriptNode* node);

			/*
				Metodo virtual para serializar el nodo a json
			*/
			virtual nlohmann::json ToJson();

			/*
				Devuelve un string con un ID unico para usar como nombre de la ventana
			*/
			virtual std::string GetStringId();

			/*
				Devuelve un puntero a la entrada de flujo del nodo
			*/
			virtual ScriptFlow* GetScriptFlow();

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


			/*
				Devuelve si el nodo va a ser serializado desde el editor
			*/
			bool IsSerialized();

			/*
				Devuelve el nombre del atributo en caso de ser serializado
			*/
			std::string GetName();


			/*
				Establece el valor de serializacion del nodo
			*/
			void SetSerialized(bool value, const std::string& str);

		private:

			bool alwaysSerialize;
			bool serialized;
			char serializedName[256];

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

		class ScriptFlow {

		private:


			ScriptFlow* next;   //Puntero al siguiente nodo en el flujo

			/*
				Lista de nodos anteriores a este. Util para notificar a dichos nodos
				cuando este sea eliminado
			*/
			std::vector<ScriptFlow*> previous;

			ScriptNode* node; //Nodo asociado

			int flowNodeSize;	//Ancho del boton de salida

			int xoffset, yoffset; //Desplazamiento del punto inicial

			bool onlyOutput; //Devuelve si el nodo debe usarse solo como salida o tambien permitir entrada


		public:

			/*
				Nodo actualmente seleccionado para continuar el flujo
			*/
			static ScriptFlow* currentSelectedFlow;


			ScriptFlow(ScriptNode* node, bool onlyOutput = false);

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


			/*
				Devuelve el nodo al que apunta el siguiente
			*/
			ScriptNode* GetNext();


			/*
				Devuelve el nodo al que apunta el siguiente
			*/
			void SetNext(ScriptFlow* next);



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
				Establece el nodo siguiente
			*/
			void SetNext(ScriptFlow* flow);

			/*
				Callback para cuando un nodo es eliminado
			*/
			void OnRemoved() override;


			/*
				Busca el nodo dentro del array de entradas y lo elimina
			*/
			void RemoveInput(ScriptNode* node) override;


			/*
				Devuelve un puntero a la entrada de flujo del nodo
			*/
			ScriptFlow* GetScriptFlow() override;


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





		class ScriptFork : public ScriptNode {

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


			/*
				Callback cuando se elimine el nodo de condicion
			*/
			void RemoveInput(ScriptNode* node) override;


			/*
				Establece el valor de la condicion
			*/
			void SetCondition(ScriptNode* condition);

			/*
				Establece la salidas de flujo
			*/
			void SetA(ScriptFlow* flow);

			/*
				Establece la salidas de flujo
			*/
			void SetB(ScriptFlow* flow);



			/*
				Devuelve un puntero a la entrada de flujo del nodo
			*/
			ScriptFlow* GetScriptFlow() override;


		private:

			Fork forkType; //El tipo de flujo del nodo
			ScriptFlow* A, * B; //Cada nodo de continuacion de flujo

			ScriptNode* condition; //Condicion para el cambio del flujo

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





		class ScriptEvent : public ScriptNode {


		private:
			std::string eventname;
			std::string stylisedName;

			ScriptFlow* flow;


			void updateAndRender() override;

		public:

			ScriptEvent(const std::string& ev);

			/*
				Convierte un nombre dado de camelCase a uno 
				mas legible separado por espacios
			*/
			static std::string StyleName(const std::string& str);

			/*
				Callback para cuando un nodo es eliminado
			*/
			virtual void OnRemoved() override;

			/*
				Devuelve un string con un ID unico (nombre del evento estilizado)
			*/
			virtual std::string GetStringId() override;


			/*
				Serializacion del valor del nodo junto con el tipo
			*/
			nlohmann::json ToJson() override;


			/*
				Devuelve el nombre del evento 
			 */
			std::string GetEventName();

			/*
				Devuelve un puntero a la entrada de flujo del nodo
			*/
			ScriptFlow* GetScriptFlow() override;
		};




		class ScriptComment: public ScriptNode {


		private:

			char comment[256];

		public:

			ScriptComment(const std::string comment);


			/*
				Devuelve un string con un ID unico (nombre del evento estilizado)
			*/
			virtual std::string GetStringId() override;


			void updateAndRender() override;

			void SetSize(float w, float h);

			/*
				Serializacion del nodo
			*/
			nlohmann::json ToJson() override;


		};



		/*
			Clase para representar la barra de herramientas en la parte superior de la ventana

			Desde la barra se puede cambiar el nombre del script, guardar, cerrar, buscar un nodo,
			y crear nodos de input y de fork... ect
		*/

		class ScriptMenuBar {

		public:

			//Dimension constante del array
			static const int CharBufferSize = 256;

		private:

			char nameBuffer[CharBufferSize]; //Buffer para introducir el nombre del script
			char nameSearch[CharBufferSize]; //Buffer para introducir la busqueda de un nodo

			bool showNodeSearch;	//Mostrar el desplegable con los resultados de la busqueda de nodos
			bool showClosePopup;	//Mostrar un popup cuando se intente cerrar la ventana con cambios pendientes

			ScriptCreation* creator; //Puntero al script creator para poder acceder a la lista de nodos existentes

			void Close();	//Cierra el pop up y cambia la ventana a la ventana principal del motor
			void Save();	//Llama a Save de script creation


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
				Cambia el valor del nombre del fichero
			*/
			void SetName(const std::string& name);

			/*
				Devuelve el puntero al buffer del nombre
			*/
			char* GetName();

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

			//Colores de la linea
			static float r, g, b, a;

			//Ancho del contorno
			static float outlineThickness;
			//Colores del contorno
			static float r_outline, g_outline, b_outline;

			//Curvatura
			static float curvature;

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
				Establece el color de la linea
			*/
			static void SetColor(float r, float g, float b);

			/*
				Restablece el color de la linea
			*/
			static void ResetColor();


			/*
				Establece el color de la linea
			*/
			static void SetOutlineColor(float r, float g, float b);

			/*
				Restablece el color de la linea
			*/
			static void ResetOutlineColor();

			/*
				Cambia el grosor del contorno de la linea
			*/
			static void SetOutlineThickness(float thickness);

			/*
				Restablece el grosor del contorno de la linea
			*/
			static void ResetOutlineThickness();

			/*
				Establece la opacidad de la linea
			*/
			static void SetAlpha(float a);

			/*
				Restablece la opacidad de la linea
			*/
			static void ResetAlpha();

			/*
				Establece la curvatura de la linea
			*/
			static void SetCurvature(float curv);


			/*
				Restablece la curvatura de la linea
			*/
			static void ResetCurvature();


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
			static float r, g, b, a;

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
			static void SetColor(float r, float g, float b);

			/*
				Cambiar el color de las lineas a su valor predeterminado
			*/
			static void ResetColor();

			/*
				Cambiar la transparencia de la linea
			*/
			static void SetAlpha(float a);

			/*
				Resetear la transparencia de la linea
			*/
			static void ResetAlpha();

			/*
				Dibuja la cuadricula sobre la ventana actual
			*/


			/*
				Cambiar el grosor estandar de la linea
			*/
			static void SetThickness(float thickness);
			/*
				Resetear el grosor estandar de la linea
			*/
			static void ResetThickness();

			/*
				Cambiar el grosor de forma proporcional de las lineas de intervalo
			*/
			static void SetIntervalScale(float scale);
			/*
				Resetear el grosor de las lineas de intervalo
			*/
			static void ResetIntevalScale();


			static void Draw();
		};

	}
};
