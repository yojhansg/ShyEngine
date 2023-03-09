#include "GenerateScriptMethod.h"
#include <iostream>
#include <filesystem>

GenerateScriptMethod::GenerateScriptMethod(std::string const& root, std::string const& output) : root(root), output(output)
{
}


void GenerateScriptMethod::AskForRoot()
{
	AskForPath("root", root);
}

void GenerateScriptMethod::AskForOutput()
{
	AskForPath("output directory", output);
}

void GenerateScriptMethod::AskForPath(std::string const& name, std::string& path)
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

GenerateScriptMethod& GenerateScriptMethod::Begin()
{
	ProcessFolder(root);
	return *this;
}

std::string GenerateScriptMethod::Result()
{
	std::string result;
	return result;
}

void GenerateScriptMethod::CreateOutputFile(std::string const& fileName, std::vector<std::string>& methods)
{
	std::ofstream stream(output + "\\" + fileName + ".txt");

	for (std::string & m : methods) {

		WriteMethod(stream, m);
	}

	stream.close();
}

void GenerateScriptMethod::CreateOutputFolder()
{
	std::filesystem::path path(output);
	std::filesystem::create_directories(output);
}


std::string GenerateScriptMethod::GetDefaultRoot()
{
	return std::filesystem::current_path().string();
}

std::string GenerateScriptMethod::GetDefaultOutput()
{
	return (std::filesystem::current_path() /= "ScriptMethods").string();
}



void GenerateScriptMethod::ProcessFolder(std::string const& path)
{
	std::filesystem::path currentPath = std::filesystem::path(path);

	for (auto const& it : std::filesystem::directory_iterator(currentPath)) {

		(this->*(it.is_directory() ? &GenerateScriptMethod::ProcessFolder : &GenerateScriptMethod::ProcessFile))(it.path().string());
	}
}

void GenerateScriptMethod::ProcessFile(std::string const& path)
{
	std::ifstream stream(path);

	bool containsPublish = false;

	std::vector<std::string> methodsFound;

	while (stream) {
		std::string line;

		std::getline(stream, line);
		line = GenerateScriptMethod::trim(line);

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


void GenerateScriptMethod::WriteMethod(std::ofstream& stream, std::string const& methodInfo)
{
	stream << "Scripting::";

	std::stringstream ss(methodInfo);

	std::string returnType, methodName, allInput;
	std::getline(ss, returnType, ' ');
	std::getline(ss, methodName, '(');

	std::getline(ss, allInput, ')');

	std::cout << "input: " << allInput << std::endl;
	std::cout << "returnType: " << returnType << std::endl;
	std::cout << "methodName: " << methodName << std::endl;

	stream << methodName << "(){\n}\n";
}