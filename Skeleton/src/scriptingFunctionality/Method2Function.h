#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define WHITESPACE "\n\r\t\f\v "

/*

TODO: 
	generar otro fichero txt para usar por el editor
	Que ese fichero sirva para facilitar la lectura de los distintos metodos creados

	Hacer que la ruta de functionManager sea relativa y no absoluta

	El nombre del metodo en el mapa no incluye el nombre de la clase
*/


/*
	Method2Function
		
	Clase auxiliar del motor. Se encarga de generar los ficheros "FunctionManager.h" y "FuncionManager.cpp".
	Estos ficheros luego se usan el motor para hacer las llamadas a los distintos metodos de los componentes.

	Se encarga de leer los ficheros en una ruta seleccionada. Dentro de cada fichero busca dentro de cada clase
	y busca por el modificador de acceso 'publish'. Por cada metodo que este marcado con dicho modificador se creara
	una funcion que lo invoque. 

	La finalidad es poder llamar a un metodo de una clase teniendo un puntero a la instancia y conociendo el nombre
	del metodo


	Orden de ejecucion:

		1: Leer todos los ficheros
		2: Generar fichero.h
		3: Generar fichero.cpp

*/

class Method2Function {

public:
	/*
		Convertir de clase con metodos a funcion:
		Ejemplo:

			class Transform{

				publish:
					void Move(Vector3 newPosition);
					Vector3 GetPosition();
			}

		Function declaration:

			Scripting::Variable Transform_Move(std::vector<Scripting::Variable>& vec);
			Scripting::Variable Transform_GetPosition(std::vector<Scripting::Variable>& vec);

		Function definition:

			Scripting::Variable Transform_Move(std::vector<Scripting::Variable>& vec){

				Transform* self = static_cast<Transform*>(vec[0]);
				
				self->Move(vec[1].value.Vector3);

				return Scripting::Variable::Null();
			}

			Scripting::Variable Transform_GetPosition(std::vector<Scripting::Variable>& vec){

				Transform* self = static_cast<Transform*>(vec[0]);

				Vector3 ret = self->GetPosition();

				return ret;
			}
	*/

	Method2Function(std::string const& root, std::string const& output);

	/*
		Abre la carpeta dada por la ruta. 
		Lee el contenido de cada fichero en la carpeta con <ProcessFile>

		Llama de forma recursiva a este metodo con todos 
		las carpetas que se encuentren en dicha ruta
	*/
	void ProcessFolder(std::string const& path);

	/*
		Lee el contenido del fichero dado
		Busca dentro del fichero todos los metodos marcados
		con el modificador de acceso <publish>
	*/
	void ProcessFile(std::string const& path);

	/*
		Llama a <AskForPath> preguntando por la ruta de los ficheros
	*/
	void AskForRoot();

	/*
		Llama a <AskForPath> preguntando por la salida de los ficheros
	*/
	void AskForOutput();

	/*
		Pregunta si una ruta dada es la correcta
	*/
	void AskForPath(std::string const& name, std::string& path);

	/*
		Comienza a procesar carpetas a partir de la carpeta <output> 
	*/

	Method2Function& Begin();

	/* TODO
		Crea los ficheros de salida del programa.
	*/
	Method2Function& CreateFunctionManagerHeader();
	Method2Function& CreateFunctionManagerContent();

	/*
		Crea la carpeta de salida en caso de que no exista
	*/
	void CreateOutputFolder();

	/*
		Genera las rutas por defecto
	*/
	static std::string GetDefaultRoot();
	static std::string GetDefaultOutput();


	struct Method {

		struct Variable {
			std::string name;
			std::string type;
		};

		std::string className;
		std::string returnType;
		std::string methodName;
		std::vector<Variable> input;

		std::string FunctionDefinition();
		std::string FunctionDeclaration();

		/*
			Dado un string conteniendo un tipo devuelve su representacion en una variable de scripting
		*/
		std::string String2ScriptingVariable(std::string& in);
	};


	/*
		Crea una instancia de <Method> dado un string 
	*/
	static Method CreateMethod(std::string const& line, std::string const& className);

private:

	/*
		Directorios del programa
	*/
	std::string root;
	std::string output;
	
	/*
		Vector con la informacion de los metodos encontrados
	*/
	std::vector<Method> methods;
	std::vector<std::string> filesToInclude;

	/*
		Metodos de eliminar espacios de cadenas de texto 
	*/

	public:
	inline std::string static ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	inline std::string static rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	inline std::string static trim(const std::string& s) {
		return rtrim(ltrim(s));
	}
};
