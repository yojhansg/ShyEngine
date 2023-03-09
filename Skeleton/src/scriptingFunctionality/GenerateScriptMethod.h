#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

#define WHITESPACE "\n\r\t\f\v"
#define publish

class GenerateScriptMethod {

public:

	GenerateScriptMethod(std::string const& root, std::string const& output);

	void ProcessFolder(std::string const& path);
	void ProcessFile(std::string const& path);

	void AskForRoot();
	void AskForOutput();

	void AskForPath(std::string const& name, std::string& path);

	GenerateScriptMethod& Begin();
	std::string Result();

	void CreateOutputFile(std::string const& fileName, std::vector<std::string>& methods);
	void CreateOutputFolder();

	void WriteMethod(std::ofstream& stream, std::string const& methodInfo);



	static std::string GetDefaultRoot();
	static std::string GetDefaultOutput();

private:

	std::string root;
	std::string output;
	std::map<std::string, std::pair<std::string, std::string>> methods;

	/*
	* Example method
	* 
	* Scripting::Variable MiMetodo(std::vector<Scripting::Variable> const& input)
	{
		Transform* tr = static_cast<Transform*>(input[0].value.pointer);
		tr->rotate(input[1].value.Float);

		std::cout << "Hola que tal" << std::endl;

		return Scripting::Variable::Null();
	}
	* 
	*/



	/*

		String utility

	*/

	std::string static ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string static rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string static trim(const std::string& s) {
		return rtrim(ltrim(s));
	}
};
