#pragma once

#include <vector>
#include <string>

class Script;

namespace CFlat {

	enum class ParamType {

		_null = 0,
		_int = 1,
		_float = 2,
		_vector2int = 4,
		_vector2float = 8,
		_vector3int = 16,
		_vector3float = 32,
		_string = 64,
		_boolean = 128,
		_box = 256
	};

	class IBox;

	struct Output {

		struct FloatVector
		{
			float a, b, c;
		};

		struct IntVector
		{
			float a, b, c;
		};

		union Values {
			bool boolean;
			int intValue;
			float floatValue;
			IntVector intVector;
			FloatVector floatVector;
			IBox* box;
		};

		std::string message;

		Values value;
		ParamType type;
	};

	/*
	*	Clase que representa cada nodo del script
	*/
	class IBox {

	protected:
		/*
		*	Comprueba si los nodos de entrada ya tienen su correspondiente output generado
		*/
		void validateInput(std::ofstream& stream);

		int operation;	//Indice de operacion
		int boxId;		//Id que representa al nodo

	public:
		IBox(int operation, int id);
		~IBox();

		bool constOutput;	//Determina si la entrada es un valor constante (en caso de ser constante no tiene que ser ejecutada en cada ciclo)

		IBox* nextBox;		//Siguiente nodo a ejecutarse
		Output output;		//Output del nodo
		std::vector<IBox*> input;	//Vector con los nodos de entrada

		std::string boxName();	//Metodo que devuelve el nombre del nodo

		//	String con la inicializacion de una variable para convertir el nodo en codigo c++
		std::string constValueOperation;

		Script* script;		//Puntero al script

		void processBox(std::ofstream& stream);		//Procesa el nodo y calcula el output
		void addInput(IBox* box);					//Añade un nuevo input al nodo

		bool used;
	};

}



