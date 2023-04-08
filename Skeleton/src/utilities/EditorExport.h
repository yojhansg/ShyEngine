#pragma once



//TODO: ver si editor componente y editor manager iria mejor delante de la palabra class porque asi no llora el compilador



/*
	Todo: Descripcion

	Para que una clase sea exportada al Editor tiene que tener la marca <EditorComponent> o tener algun
	atributo marcado con <reflect> o metodo marcado con <publish>

	Si una clase no esta marcada como <EditorComponent> pero tiene algun metodo publicado o algun atributo
	marcado con reflexion entonces la clase se tratara como si fuera un <EditorComponent>

	Pueden haber componentes exportados sin tener nigun metodo o atributo publicado
	pero tiene que estar marcado con <EditorComponent>



	Pueden haber clases anidadas dentro de la clase pero tienen que estar definidas despues de todas
	las etiquetas <publish> y <reflect>
*/
#define EditorComponent class


/*
	Todo: Descripcion


	Esta clase no puede tener atributos marcados con reflect.
	En caso de marcar un metodo con publish pero no haber marcado la clase como <EditorManager>, la clase 
	tratara como si fuera un <EditorComponent> pudiendo generar errores o comportamientos no deseados

	<ECSReader> ignora espacios de nombre, por lo que cuando se generen los managers se tendran que manejar los
	espacios de nombres a mano

*/
#define EditorManager class


/*
	Publicar metodos de componentes.

	Para poder publicar un metodo basta con poner el modificador de
	acceso <publish> encima de los metodos que se quieran publicar.

	Condiciones para poder publicar un metodo:

		-No puede devolver un valor <const> o por referencia
		-No puede recibir un parametro <const> o por referencia
		-El nombre del metodo no puede contener una barra baja
		-El tipo de retorno o de los parametros no puede tener un nombre
		compuesto por un espacio de nombres o una clase (pe. usar Vector2D en vez Utilities::Vector2d)
		-En caso de ser un puntero, el asterisco no puede estar separado del nombre mediante un espacio
		-No puede haber atributos dentro del modificador publish
		-No pueden haber comentarios de asterisco. Para comentar usar doble barra (//)
		-Los tipos de entrada o salida tienen que ser los tipos permitidos. (pe. no se pueden tener enums por ejemplo)
		-No se puede usar sobre metodos estaticos
		-No se permite la sobrecarga de metodos

*/
#define publish public

/*
	Para poder tener reflexion sobre un atributo basta con poner la marca <reflect>
	delante del atributo en cuestion. No es necesario que el atributo sea publico para que funcione

	Condiciones para poder publicar un metodo:

		-
		-No puede recibir un parametro <const> o por referencia
		-El nombre del metodo no puede contener una barra baja
		-No se puede usar <reflect> en varios atributos simultaneamente (pe. No se puede usar en 'reflect int x, y;'.)
		-No pueden haber <reflect> en la misma linea (pe. No se podria usar 'reflect x; reflect y;'. Tendrian que estar en lineas separadas)
		-Solo se permite la reflexion sobre los tipos permitidos. (pe. no se pueden tener enums por ejemplo)

*/
#define reflect friend class ClassReflection;



/*
	Forward declaration de <ClassReflection> para evitar errores de link 	
*/
class ClassReflection;


//TODO: implementar el paso por referencia para evitar copias de las cosas
//#define REF const&