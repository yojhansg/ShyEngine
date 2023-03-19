#include "ECSReader.h"
#include "ClassCreator.h"
#include <filesystem>

ECSReader::ECSReader(std::string const& root) : root(root)
{
	output = root + "\\ECSUtilities";
}

void ECSReader::AskForRoot()
{
	AskForPath("root", root);
	output = root + "\\ECSUtilities";
}

void ECSReader::AskForOutput()
{
	AskForPath("output directory", output);
}

void ECSReader::AskForPath(std::string const& name, std::string& path)
{
	bool pathAccepted = false;
	while (!pathAccepted)
	{
		std::cout << "Current " << name << ": " << path << std::endl;
		std::cout << "Confirm? (y/n)" << std::endl;

		std::string line;
		std::getline(std::cin, line);

		if (line != "y" && line != "n") {
			return AskForPath(name, path);
		}

		if (line == "y")
			break;

		std::cout << "Enter new " << name << ": " << std::endl;
		std::getline(std::cin, path);
	}
}

ECSReader& ECSReader::Read()
{
	ProcessFolder(root);
	return *this;
}


ECSReader& ECSReader::Method2Function() {

	return CreateFunctionManagerHeader().CreateFunctionManagerSource();
}


void ECSReader::CreateOutputFolder()
{
	std::filesystem::path path(output);
	std::filesystem::create_directories(output);
}


std::string ECSReader::GetDefaultRoot()
{
	return R"(C:\Users\sryoj\Documents\TFG\Skeleton\src\ecs)";
}


void ECSReader::ProcessFolder(std::string const& path)
{
	std::filesystem::path currentPath = std::filesystem::path(path);

	for (auto const& it : std::filesystem::directory_iterator(currentPath)) {

		(this->*(it.is_directory() ? &ECSReader::ProcessFolder : &ECSReader::ProcessFile))(it.path().string());
	}
}

void ECSReader::ProcessFile(std::string const& path)
{
	std::ifstream stream(path);

	bool containsPublish = false;
	bool fileIncluded = false;
	std::string currentClassName = "";


	while (stream) {
		std::string line;

		std::getline(stream, line);
		line = ECSReader::trim(line);

		if (line == "")
			continue;

		//TODO: esto no hacerlo asi, mejor hacer que importe el fichero .h y ya
		if (line.contains("ECS_Version")) {

			ECS_Version = trim(line.substr(line.find("ECS_Version") + 12));
			continue;
		}

		if (line.rfind("//", 0) == 0) //Ignora comentarios
			continue;

		if (line == "publish:") {
			containsPublish = true;
			continue;
		}

		if (line == "public:" || line == "private:" || line == "protected:" || line == "}" || line == "};") {
			containsPublish = false;
			continue;
		}

		if (line.contains("class")) {

			if (line.contains(";"))
				continue;

			std::string className = line.erase(0, line.find("class") + 5);

			if (className.contains("{")) {
				className = className.substr(0, className.find("{"));
			}

			size_t space = className.find(":");
			if (space != std::string::npos)
				className = className.substr(0, space);


			currentClassName = ECSReader::trim(className);

			continue;
		}

		if (line.rfind("reflect", 0) == 0) {

			std::string removeReflect = line.substr(8);

			int blankSpace = removeReflect.find(" ");

			std::string attReturnType = removeReflect.substr(0, blankSpace);
			std::string attNameWithComa = removeReflect.substr(blankSpace + 1);
			std::string attName = attNameWithComa.substr(0, attNameWithComa.find(";"));

			attributes[currentClassName].push_back({ attReturnType, attName });

			//TODO: ver si deberia separar la reflexion de la funcionalizacion a la hora de incluir ficheros
			if (!fileIncluded) {

				filesToInclude.push_back(path);
				fileIncluded = true;
			}

			continue;
		}

		if (currentClassName.size() > 0 && containsPublish) {

			methods[currentClassName].push_back(CreateMethod(line, currentClassName));

			if (!fileIncluded) {

				filesToInclude.push_back(path);
				fileIncluded = true;
			}
		}
	}

	stream.close();
}



ECSReader::Method ECSReader::CreateMethod(std::string const& line, std::string const& className)
{
	Method method;

	method.className = className;

	std::string l = trim(line);

	size_t nextWord = l.find(" ");
	method.returnType = l.substr(0, nextWord);

	l = l.substr(nextWord + 1);
	nextWord = l.find("(");
	method.methodName = trim(l.substr(0, nextWord));

	l = l.substr(nextWord + 1);

	bool isType = true;
	std::string returnType;
	std::string paramName;
	for (char c : l) {
		if (c == ' ') {

			if (returnType.size() > 0)
				isType = false;

			continue;
		}


		if (c == ',') {
			method.input.push_back({ paramName, returnType });
			returnType = "";
			paramName = "";

			isType = true;
			continue;
		}

		if (c == ')') {
			if (returnType.size() > 0)
				method.input.push_back({ paramName, returnType });
			break;
		}

		if (isType)
			returnType += c;
		else paramName += c;
	}

	return method;
}




/*
Example:

	class Transform {

		publish:
			void Move(Vector3 newPosition);
			Vector3 GetPosition();
		}

		Function declaration :

		Scripting::Variable Transform_Move(std::vector<Scripting::Variable>& vec);
		Scripting::Variable Transform_GetPosition(std::vector<Scripting::Variable>& vec);

		Function definition :

		Scripting::Variable Transform_Move(std::vector<Scripting::Variable>& vec) {

			Transform* self = static_cast<Transform*>(vec[0]);

			self->Move(vec[1].value.Vector3);

			return Scripting::Variable::Null();
		}

		Scripting::Variable Transform_GetPosition(std::vector<Scripting::Variable>& vec) {

			Transform* self = static_cast<Transform*>(vec[0]);

			Vector3 ret = self->GetPosition();

			return ret;
		}

*/



#define BLANK " "
#define TAB "\t"
#define NEWLINE "\n"
#define VARIABLE "Scripting::Variable"
#define VECTOR "std::vector<" VARIABLE ">const& vec"
#define CAST(className, cast) "static_cast<" + className + "*>(" + cast + ")"


std::string ECSReader::Method::FunctionName()
{
	return className + "_" + methodName;
}

std::string ECSReader::Method::FunctionDeclaration()
{
	std::stringstream definition;

	definition << VARIABLE BLANK;

	definition << FunctionName();

	definition << "(" VECTOR ");" NEWLINE;

	return definition.str();
}

std::string ECSReader::Method::FunctionDefinition()
{
	std::stringstream declaration;

	declaration << VARIABLE BLANK;

	declaration << FunctionName();

	declaration << "(" VECTOR "){" NEWLINE;

	declaration << TAB << className << "* self = " CAST(className, "vec[0].value.pointer") ";" NEWLINE;

	declaration << TAB;
	if (returnType != "void") {

		declaration << returnType << " ret = ";
	}

	declaration << "self->" << methodName << "(";

	int i = 1;
	for (auto& in : input) {

		declaration << "vec[" << i << "]." << String2ScriptingVariable(in.type) << ", ";

		i++;
	}

	if (input.size() > 0)
		declaration.seekp(-2, declaration.cur);
	declaration << ");" NEWLINE;


	if (returnType != "void")
		declaration << TAB "return ret;" NEWLINE;
	else
		declaration << TAB "return " VARIABLE "::Null();" NEWLINE;

	declaration << "}" NEWLINE;

	return declaration.str();
}


std::string ECSReader::Method::String2ScriptingVariable(std::string& in)
{
	if (in == "int")
		return "value.Int";
	if (in == "float")
		return "value.Float";
	if (in == "bool")
		return "value.Bool";
	if (in == "char")
		return "value.Char";

	return "value." + in;
}


ECSReader& ECSReader::CreateFunctionManagerHeader()
{
	std::ofstream h(output + "/FunctionManager.h");

	h << ClassCreator("FunctionManager")
		.IncludeAbsolutes({ "map", "string", "vector" })
		.IncludeRelative("Scripting/Variable.h")
		.Empty(3)
		.AddDefine("ECSfunc_Version", ECS_Version)
		.AddComment("Creation time : " __TIMESTAMP__)
		.Empty()
		.AddLine("typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);")
		.Empty()
		.BeginClass()
		.Public()
		.AddMethod("void", "CreateFunctionMap", { {"std::map<std::string, CallableFunction>&", "map"} })
		.EndClass()
		.Empty(3)
		.Header();

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			h << method.FunctionDeclaration();
		}
	}

	h.close();
	return *this;
}

ECSReader& ECSReader::CreateFunctionManagerSource()
{
	std::ofstream cpp(output + "/FunctionManager.cpp");

	cpp << R"(#include "FunctionManager.h"

)";

	cpp << "//Creation time: " << __TIMESTAMP__ << "\n";

	for (auto& file : filesToInclude) {

		auto filename = std::filesystem::path(file);
		cpp << "#include <Components/" << filename.filename().string() << ">\n";
	}

	cpp << R"(

using namespace ECS;

void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

)";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			std::string functionName = method.FunctionName();
			cpp << "\tmap.emplace(\"" << functionName << "\"," << functionName << ");\n";
		}
	}

	cpp << "\n};\n";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			cpp << method.FunctionDefinition();
		}
	}

	cpp.close();
	return *this;
}


ECSReader& ECSReader::ClassReflection()
{
	std::ofstream h(output + "/ClassReflection.h  ");
	std::ofstream cpp(output + "/ClassReflection.cpp");

	ClassCreator creator = ClassCreator("ClassReflection");

	creator.IncludeAbsolutes({ "string" , "map" });



	for (auto& file : filesToInclude) {

		auto filename = std::filesystem::path(file);
		creator.IncludeAbsolute("Components/" + filename.filename().string());
	}

	creator.Empty()
		.AddComment("Creation time : " __TIMESTAMP__)
		.Empty()
		.AddLine("using namespace ECS;")
		.BeginClass()
		//TODO: mapa a funciones para simplificar aun mas el proceso.AddAtribute("std::map<std::string, ")
		.AddMethod("void", "hola", { {} }, "");

	for (auto& className : attributes) {

		std::stringstream method;
		for (auto& attribute : className.second) {

			//TODO hacer un metodo que convierta de string a un valor
			method << "\tif(map.contains(\"" << attribute.name << "\"))\n";
			method << "\t\tself->" << attribute.name << "= 10" << ";\n";
		}


		creator.AddMethod("void", "Reflect" + className.first, { {className.first + "*", "self"},
			{"std::map<std::string, std::string> const&", "map"} }, method.str(), true);
	}


	//TODO: Buscar las clases que tengo que añadir para reflexionar

	creator.EndClass();

	h << creator.Header();
	cpp << creator.Source();

	h.close();
	cpp.close();
	return *this;
}



#include "json.hpp"
using namespace nlohmann;

ECSReader& ECSReader::Convert2JSON()
{
	/*
		Ejemplo del JSON

			{
			"Transform" : {
				"Move" : {
					"input" = {
						Vector3 Move;
					},
					"return" : "int"
				}
			},
		}
	*/


	json root;

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			root[currentClass.first][method.methodName] =
			{
				{"return", method.returnType}
			};


			json inputJson;

			for (auto& input : method.input) {
				inputJson +=
				{
					{"type", input.type},
					{ "name", input.name }
				};
			}

			if (!inputJson.is_null())
				root[currentClass.first][method.methodName]["input"] = inputJson;
		}
	}

	std::ofstream fmJSON(output + "/FunctionManager.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}




