#include "CFlatBasics.h"

#include <iostream>

CFlat::IBox* CFlat::Attributes::createAttribute(CFlat::ParamType type, int id) {

	IBox* box = new IBox(-1, id);
	box->output.type = type;

	box->constOutput = true;
	return box;
}

CFlat::IBox* CFlat::Attributes::createFloat(float value, int id) {

	CFlat::IBox* box = createAttribute(ParamType::_float, id);
	box->output.value.floatValue = value;
	box->constValueOperation = "float " + box->boxName() + " = " + std::to_string(value) + ";\n";
	return box;
}

CFlat::IBox* CFlat::Attributes::createInt(int value, int id) {

	CFlat::IBox* box = createAttribute(ParamType::_int, id);
	box->output.value.intValue = value;
	box->constValueOperation = "int " + box->boxName() + " = " + std::to_string(value) + ";\n";
	return box;
}

CFlat::IBox* CFlat::Attributes::createBoolean(int value, int id) {

	CFlat::IBox* box = createAttribute(ParamType::_boolean, id);
	box->output.value.boolean = value != 0;
	box->constValueOperation = "bool " + box->boxName() + " = " + std::to_string(value) + ";\n";
	return box;
}

CFlat::IBox* CFlat::Attributes::createString(std::string value, int id)
{
	CFlat::IBox* box = CFlat::Attributes::createAttribute(CFlat::ParamType::_string, id);
	box->output.message = value;
	box->constValueOperation = "std::string " + box->boxName() + " = \"" + value + "\";\n";
	return box;
}


namespace CFlat {


	Split::Split(int op, int id) : IBox(op, id)
	{
		constOutput = false;
		otherRoute = nullptr;
	}

	void Split::setOtherRoute(IBox* o)
	{
		output.type = ParamType::_box;
		output.value.box = nextBox;
		otherRoute = o;
	}
}
