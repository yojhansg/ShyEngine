#include "ScriptAttribute.h"

void ScriptAttribute::generateAttribute(string name, bool value)
{
	booleans.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, int value)
{
	integers.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, float value)
{
	decimals.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, string value)
{
	strings.emplace(name, value);
}



void ScriptAttribute::updateAttribute(string name, int value)
{
	integers[name] = value;
}

void ScriptAttribute::updateAttribute(string name, float value)
{
	decimals[name] = value;
}

void ScriptAttribute::updateAttribute(string name, bool value)
{
	booleans[name] = value;
}

void ScriptAttribute::updateAttribute(string name, string value)
{
	strings[name] = value;
}



void ScriptAttribute::retreiveAttribute(string name, bool& value)
{
	value = booleans.at(name);
}

void ScriptAttribute::retreiveAttribute(string name, int& value)
{
	value = integers.at(name);
}

void ScriptAttribute::retreiveAttribute(string name, float& value)
{
	value = decimals.at(name);
}

void ScriptAttribute::retreiveAttribute(string name, string& value)
{
	value = strings.at(name);
}
