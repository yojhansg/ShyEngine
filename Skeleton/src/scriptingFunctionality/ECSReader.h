#pragma once

/*
	ECSReader
		
	Clase auxiliar del motor. Se encarga de generar los ficheros "FunctionManager.h" y "FuncionManager.cpp".
	Estos ficheros luego se usan el motor para hacer las llamadas a los distintos metodos de los componentes.

	Se encarga de leer los ficheros en una ruta seleccionada. Dentro de cada fichero busca dentro de cada clase
	y busca por el modificador de acceso 'publish'. Por cada metodo que este marcado con dicho modificador se creara
	una funcion que lo invoque. 

	La finalidad es poder llamar a un metodo de una clase teniendo un puntero a la instancia y conociendo el nombre
	del metodo
*/



/*
	TODO:

	Cambiar la forma de leer la version

	Añadir los valores que permiten reflexion al json con informacion

	Cambiar nombre de salida del JSON


	Contenido a generar:

		Un json con la informacion del proyecto

			-Componentes:
				-Atributos publicos de los componentes


*/

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>


class ECSReader {

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

	ECSReader(std::string const& root);

	/*
		Comienza a procesar carpetas a partir de la carpeta <output>
	*/

	ECSReader& Read();

	/*
		Genera la clase <FunctionManager> con las distintas funciones marcadas como <publish> del ECS
	*/
	ECSReader& Method2Function();
	
	/*
		Genera la clase <FunctionManager> con los atributos marcados como <reflect> del ECS
	*/
	ECSReader& ClassReflection();

	/*
		Genera un fichero JSON con la informacion de las funciones del ECS
	*/
	ECSReader& GenerateFunctionJSON();

	/*
		Genera un fichero JSON con la informacion de los atributos del ECS
	*/
	ECSReader& GenerateAttributeJSON();

	/*
		Genera un fichero JSON con una lista con todos los componentes del ECS
	*/
	ECSReader& GenerateComponentsJSON();


	/*
		Genera un fichero JSON con una lista con todos los componentes del ECS
	*/
	ECSReader& GenerateManagersJSON();

	/*
		TODO: Comentario descriptivo
	*/
	ECSReader& ComponentFactory();

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
	

	void ManageInheritance();

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
		Genera las rutas por defecto
	*/
	static std::string GetDefaultRoot();


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


		std::string ManagerFunctionDeclaration();

		/*
			Nombre de la funcion en el scripting
		*/
		std::string ScriptName();
		/*
			Nombre de la funcion
		*/
		std::string FunctionName();


		/*
			Dado un string conteniendo un tipo devuelve su representacion en una variable de scripting
		*/
		std::string String2ScriptingVariable(std::string const& in);


		/*
			Dado un string conteniendo un tipo devuelve su representacion el tipo de enum de variable
		*/
		std::string String2ScriptingEnum(std::string const& in);

		/*
			Dado un string conteniendo un tipo devuelve si el tipo en cuestion es de tipo variable
		 */
		bool IsVariable(std::string const& in);

		/*
			Variable de comprobacion en caso en caso de que haya habido un error generando el metodo
		*/
		bool valid = true;

		/*
			Conocer la clase de la que hereda el componente
		*/
		std::string inheritance = "";
	};

	/*
		Crea una instancia de <Method> dado un string 
	*/
	static Method CreateMethod(std::string const& line, std::string const& className);


	struct Attribute {

		std::string type;
		std::string name;

		std::string TypeConversion(std::string const& convertName);
	};

	/*
		Ajusta los tipos para ser procesados por el motor
	*/
	std::string ProcessType(std::string const& input);

private:

	/*
		Directorios del programa
	*/
	std::string root;
	std::string output;
	std::string ECS_Version;
	/*
		Vector con la informacion de los metodos encontrados
	*/
	std::unordered_set<std::string> allComponents;

	std::vector<std::string> filesToInclude;
	std::vector<std::string> managerFiles;
	std::map<std::string, std::vector<Method>> methods;
	std::map<std::string, std::vector<Attribute>> attributes;
	std::map<std::string, std::vector<Method>> managerMethods;

	std::map<std::string, std::string> classInheritance;

	std::vector<std::pair<std::string, std::string>> componentsWithPaths;
	/*
		Crea la carpeta de salida en caso de que no exista
	*/
	void CreateOutputFolder();

	/*
		Genera el fichero .h de la clase <FunctionManager>
	*/
	ECSReader& CreateFunctionManagerHeader();
	
	/*
		Genera el fichero .cpp de la clase <FunctionManager>
	*/
	ECSReader& CreateFunctionManagerSource();


	/*
		Devuelve el tiempo actual
	*/
	std::string GetTimeStamp();
	
};




