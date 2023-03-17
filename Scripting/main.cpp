#include <iostream>
#include "Script.h"
#include "ScriptAttribute.h"
#include "OperatorManager.h"

#include "FlowManager.h"


using CFlat::IBox;

int main() {

	/*
	*	TODO:
	*	Variables en cada script
	*	Encadenar instrucciones
	*
	*	Queda manejar las referencias a otras entidades de la escena
	*
	*	Hay que eliminar el output de los nodos al terminar el ciclo
	*
	*
	*	El bug de que si por algun motivo añades un input todo peta xd
	*
	*	Cosas que necesita el lenguaje:
	*
	*	Operaciones enlazadas (una operacion que apunta a la siguiente)
	*	Atributos
	*	Metodos
	*	Condicionales
	*	Bucles
	*	Arrays
	*	Estructuras
	*
	*
	*
	*
			*
			*	Para agrupar objetos se hara con una coleccion
			*
			*	Una coleccion es un conjunto de atributos
			*
			*
				Operaciones que se pueden hacer con una coleccion

				consultar un elemento
				establecer un elemento

				añadir un elemento
				eliminar un elemento

				consultar el tamaño del array




				puedo hacer una cosa parecida a clases haciendo que los arrays no tengan tipo

				un set es un conjunto de atributos

				puedo hacer que un array sea:

				[ elem1, elem2, [ Set ] , elem4]

				Y haciendo que set sea un array pero con un tamaño fijo establecido previamente

				Entonces puedo hacer que un array sea un vector de sets, y si uno de los elementos no es un set que tenga un tamaño 0





				*
				*	Para hacer las llamadas a metodos:
				* 
				*	Hay que establecer un nodo como un punto de entrada
				*	Este punto de entrada recibe un array de nodos como input
				* 
				* 


				Esto tiene un problema y es que funcionaria si solamente el primer nodo necesitase el input
				Si un nodo intermedio lo quisiera habria que hacer otra cosa

				Tiene que haber una clase que funcione como la pila del metodo
				Mas adelante para poder usar esos valores se puede llamar con un nodo que funcione como getParamFloat( nombre )


			 */

	//TODO: ahora las variables se tienen que hacer de forma diferente porque no quiero que sean valores compilados (se tienen que leer)
	//Entonces las variables se pueden guardar en otro archivo diferente



	CFlat::OperatorManager::Initialise();
	CFlat::FlowManager flowManager;

	Script script{};


	script.setupScript("NivelMireLol");
	//script.init(output);

	//Todo:
	// 
	// Ahora los constantes requieren un tipo de operacion nueva
	// Para que se pinte la variable donde corresponda
	// 
	
	

	return 0;
}

