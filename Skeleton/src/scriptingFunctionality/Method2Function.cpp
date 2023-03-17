#include "Method2Function.h"
#include <iostream>
#include <filesystem>


Method2Function::Method2Function(std::string const& root) : root(root)
{
	output = root + "\\FunctionManager";



}


void Method2Function::AskForRoot()
{
	AskForPath("root", root);
	output = root + "\\FunctionManager";
}

void Method2Function::AskForOutput()
{
	AskForPath("output directory", output);
}

void Method2Function::AskForPath(std::string const& name, std::string& path)
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

Method2Function& Method2Function::Begin()
{
	ProcessFolder(root);
	return *this;
}



void Method2Function::CreateOutputFolder()
{
	std::filesystem::path path(output);
	std::filesystem::create_directories(output);
}


std::string Method2Function::GetDefaultRoot()
{
	return R"(C:\Users\sryoj\Documents\TFG\Skeleton\src\ecs)";
}


void Method2Function::ProcessFolder(std::string const& path)
{
	std::filesystem::path currentPath = std::filesystem::path(path);

	for (auto const& it : std::filesystem::directory_iterator(currentPath)) {

		(this->*(it.is_directory() ? &Method2Function::ProcessFolder : &Method2Function::ProcessFile))(it.path().string());
	}
}

void Method2Function::ProcessFile(std::string const& path)
{
	std::ifstream stream(path);

	bool containsPublish = false;
	bool fileIncluded = false;
	std::string currentClassName = "";


	while (stream) {
		std::string line;

		std::getline(stream, line);
		line = Method2Function::trim(line);

		if (line == "")
			continue;

		if (line.contains("ECS_Version")) {

			ECS_Version = trim(line.substr(line.find("ECS_Version") + 12));
			continue;
		}

		if (line.contains("//"))
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


			currentClassName = Method2Function::trim(className);

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



Method2Function::Method Method2Function::CreateMethod(std::string const& line, std::string const& className)
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


std::string Method2Function::Method::FunctionName()
{
	return className + "_" + methodName;
}

std::string Method2Function::Method::FunctionDeclaration()
{
	std::stringstream definition;

	definition << VARIABLE BLANK;

	definition << FunctionName();

	definition << "(" VECTOR ");" NEWLINE;

	return definition.str();
}

std::string Method2Function::Method::FunctionDefinition()
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



std::string Method2Function::Method::String2ScriptingVariable(std::string& in)
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



Method2Function& Method2Function::CreateFunctionManagerHeader()
{
	std::ofstream h(output + "/FunctionManager.h");

	h << R"(#pragma once

#include <map>
#include <string>
#include <vector>
#include "Scripting/Variable.h"

)";

	h << "#define ECSfunc_Version " << ECS_Version << "\n";
	h << "//Creation time: " << __TIMESTAMP__ << "\n";

	h << R"(
typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager {

public:

	static void CreateFunctionMap(std::map<std::string, CallableFunction>& map);
};

)";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			h << method.FunctionDeclaration();
		}
	}


	h.close();
	return *this;
}

Method2Function& Method2Function::CreateFunctionManagerContent()
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


#include "json.hpp"
using namespace nlohmann;

Method2Function& Method2Function::CreateFunctionManagerJSON()
{

	/*
		Ejemplo del JSON


			{
			"Transform" = {
				"Move" = {
					"input" = {

					},
					"return" = "int"
				}
			},


		}

	*/


	json root;

	for (auto& currentClass : methods) {

		int idx = 0;
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

			idx++;
		}
	}


	std::cout << root.dump(4) << std::endl;

	std::ofstream fmJSON(output + "/FunctionManager.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}




