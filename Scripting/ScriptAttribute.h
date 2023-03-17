#pragma once

#include <vector>
#include <unordered_map>
#include <string>

using std::string;

class Output;

class ScriptAttribute
{
	/*
		Tipos que se pueden guardar:
			+bool
			+int
			+float
			+vector2
			+vector3
			+string
	*/

	//TODO: hacer variables privadas o publicas

	std::unordered_map<string, bool> booleans;
	std::unordered_map<string, int> integers;
	std::unordered_map<string, float> decimals;
	std::unordered_map<string, string> strings;

	std::unordered_map<string, std::vector<Output*>> arrays;

public:



	void generateAttribute(string name, bool value);
	void generateAttribute(string name, int value);
	void generateAttribute(string name, float value);
	void generateAttribute(string name, string value);

	void updateAttribute(string name, bool value);
	void updateAttribute(string name, int value);
	void updateAttribute(string name, float value);
	void updateAttribute(string name, string value);

	void retreiveAttribute(string name, bool& value);
	void retreiveAttribute(string name, int& value);
	void retreiveAttribute(string name, float& value);
	void retreiveAttribute(string name, string& value);
};

