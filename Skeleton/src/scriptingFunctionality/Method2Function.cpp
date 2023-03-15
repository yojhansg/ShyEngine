#include "Method2Function.h"
#include <iostream>
#include <filesystem>


Method2Function::Method2Function(std::string const& root, std::string const& output) : root(root), output(output)
{
}


void Method2Function::AskForRoot()
{
	AskForPath("root", root);
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

void Method2Function::CreateOutputFile(std::string const& fileName, std::vector<std::string>& methods)
{
	std::ofstream stream(output + "\\" + fileName + ".txt");

	for (std::string& m : methods) {

		//WriteMethod(stream, m);
	}

	stream.close();
}

void Method2Function::CreateOutputFolder()
{
	std::filesystem::path path(output);
	std::filesystem::create_directories(output);
}


std::string Method2Function::GetDefaultRoot()
{
	return std::filesystem::current_path().string();
}

std::string Method2Function::GetDefaultOutput()
{
	return (std::filesystem::current_path() /= "ScriptMethods").string();
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

	std::string currentClassName = "";

	std::vector<std::string> methodsFound;

	while (stream) {
		std::string line;

		std::getline(stream, line);
		line = Method2Function::trim(line);

		if (line == "")
			continue;

		if (line == "publish") {
			containsPublish = true;
			continue;
			//std::cout << "encontrado en " << path << std::endl;
		}

		if (containsPublish) {
			methodsFound.push_back(line);
		}
	}

	if (containsPublish) {
		CreateOutputFile(std::filesystem::path(path).filename().replace_extension().string(), methodsFound);
	}


	stream.close();
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
#define UNDERSCORE "_"
#define VARIABLE "Scripting::Variable"
#define VECTOR "std::vector<" VARIABLE ">& vec"
#define CAST(className) "static_cast<" + className + "*>(self)"


std::string Method2Function::Method::FunctionDeclaration()
{
	std::stringstream definition;

	definition << VARIABLE BLANK;

	definition << className << UNDERSCORE << methodName;

	definition << "(" VECTOR ");";

	return definition.str();
}


std::string Method2Function::Method::FunctionDefinition()
{
	std::stringstream declaration;

	declaration << FunctionDeclaration();

	declaration.seekp(-1, declaration.cur);

	declaration << "{" NEWLINE;

	declaration << TAB << className << "* self = " CAST(className) ";" NEWLINE;

	declaration << TAB;
	if (returnType != "void") {

		declaration << returnType << "* ret = ";
	}

	declaration << "self->" << methodName << "(";

	int i = 0;
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
