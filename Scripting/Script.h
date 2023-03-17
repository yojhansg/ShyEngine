#pragma once

#include <string>
#include <fstream>
#include "ScriptAttribute.h"

using std::string;

namespace CFlat {

	class IBox;
	class FlowManager;
}

class Script
{
	friend CFlat::FlowManager;
private:
	CFlat::IBox* initBox;
	CFlat::IBox* updateBox;

	void iteration(CFlat::IBox* startingBox, std::ofstream& stream);


public:
	ScriptAttribute attributes;

	void setupScript(std::string script);

	void init(std::ofstream& stream);

	void update(std::ofstream& stream);
};

