#include "OperatorManager.h"
#include "CFlat.h"
#include "CFlatBasics.h"
#include <iostream>
#include <math.h>

#include "Script.h"
#include "ScriptAttribute.h"

CFlat::OperatorManager::Funct CFlat::OperatorManager::operations[CFlat::OperatorManager::OperatorCount];

void CFlat::OperatorManager::Initialise()
{
	operations[Operations::Print] = BoxOperations::IO::Print;

	operations[Operations::If] = BoxOperations::Control::If;
	operations[Operations::Loop] = BoxOperations::Control::Loop;

	operations[Operations::Add] = BoxOperations::Math::Add;
	operations[Operations::Subtract] = BoxOperations::Math::Subtract;
	operations[Operations::Multiply] = BoxOperations::Math::Multiply;
	operations[Operations::Division] = BoxOperations::Math::Division;
	operations[Operations::Power] = BoxOperations::Math::Power;
	operations[Operations::SquareRoot] = BoxOperations::Math::SquareRoot;
	operations[Operations::Mod] = BoxOperations::Math::Mod;
	operations[Operations::PlusOne] = BoxOperations::Math::PlusOne;

	operations[Operations::Equal] = BoxOperations::Logic::Equals;
	operations[Operations::Different] = BoxOperations::Logic::Different;
	operations[Operations::Minor] = BoxOperations::Logic::Minor;
	operations[Operations::Greater] = BoxOperations::Logic::Greater;
	operations[Operations::MinorEqual] = BoxOperations::Logic::MinorEqual;
	operations[Operations::GreaterEqual] = BoxOperations::Logic::GreaterEqual;
	operations[Operations::And] = BoxOperations::Logic::And;
	operations[Operations::Or] = BoxOperations::Logic::Or;
	operations[Operations::Not] = BoxOperations::Logic::Not;

	operations[Operations::getInteger] = BoxOperations::Attributes::getInteger;
	operations[Operations::setInteger] = BoxOperations::Attributes::setInteger;
	operations[Operations::getDecimal] = BoxOperations::Attributes::getDecimal;
	operations[Operations::setDecimal] = BoxOperations::Attributes::setDecimal;
	operations[Operations::getBoolean] = BoxOperations::Attributes::getBoolean;
	operations[Operations::setBoolean] = BoxOperations::Attributes::setBoolean;
	operations[Operations::getString] = BoxOperations::Attributes::getString;
	operations[Operations::setString] = BoxOperations::Attributes::setString;

}



void CFlat::OperatorManager::ProccessOperation(int operationIdx, IBox* node, std::ofstream& stream)
{
	if (operationIdx < 0) {
		stream << node->constValueOperation;
		return;
	}

	operations[operationIdx](node, stream);
}

void CFlat::BoxOperations::IO::Print(IBox* node, std::ofstream& stream)
{
	Output output = node->input[0]->output;
	switch (output.type) {
	case ParamType::_float:
		std::cout << output.value.floatValue;
		break;

	case ParamType::_int:
		std::cout << output.value.intValue;
		break;
	case ParamType::_string:
		std::cout << output.message;
		break;
	}

	std::cout << std::endl;

	stream << "std::cout << " << node->input[0]->boxName() << " << std::endl;\n";
}


void CFlat::BoxOperations::Control::If(IBox* node, std::ofstream& stream)
{
	//TODO hacer bien la condicion del if
	//bool condition = node->input[0]->output.value.boolean;

	bool condition = node->input[0]->output.value.floatValue > 0;

	Split* split = static_cast<Split*>(node);

	node->nextBox = condition ? node->output.value.box : split->otherRoute;

	stream << "if(" << node->input[0]->boxName() << "){\n }";
}

void CFlat::BoxOperations::Control::Loop(IBox* node, std::ofstream& stream)
{
	CFlat::Split* split = static_cast<CFlat::Split*>(node);
	CFlat::IBox* conditionNode = node->input[0];

	bool range = false;
	if (conditionNode->output.type == ParamType::_int) {

		range = true;
	}

	int n = 0;
	while ((range && n < conditionNode->output.value.intValue) || (!range && conditionNode->output.value.boolean)) {

		CFlat::IBox* box = split->otherRoute;

		while (box != nullptr) {

			box->processBox(stream);
			box = box->nextBox;
		}

		if (!range)
			conditionNode->processBox(stream);

		n++;
	}

}


void CFlat::BoxOperations::Logic::Equals(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_boolean:
		node->output.value.boolean = node->input[0]->output.value.boolean == node->input[1]->output.value.boolean;
		break;
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue == node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue == node->input[1]->output.value.floatValue;
		break;
	case ParamType::_string:
		node->output.value.boolean = node->input[0]->output.message == node->input[1]->output.message;
		break;
	}

	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "==" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::Different(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_boolean:
		node->output.value.boolean = node->input[0]->output.value.boolean != node->input[1]->output.value.boolean;
		break;
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue != node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue != node->input[1]->output.value.floatValue;
		break;
	case ParamType::_string:
		node->output.value.boolean = node->input[0]->output.message != node->input[1]->output.message;
		break;
	}

	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "!=" << node->input[1]->boxName() << ";\n";
}



void CFlat::BoxOperations::Logic::Minor(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue < node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue < node->input[1]->output.value.floatValue;
		break;
	}

	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "<" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::Greater(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue > node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue > node->input[1]->output.value.floatValue;
		break;
	}
	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << ">" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::GreaterEqual(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue >= node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue >= node->input[1]->output.value.floatValue;
		break;
	}
	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << ">=" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::MinorEqual(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue <= node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue <= node->input[1]->output.value.floatValue;
		break;
	}
	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "<=" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::And(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = node->input[0]->output.value.boolean && node->input[1]->output.value.boolean;

	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "&&" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::Or(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = !node->input[0]->output.value.boolean || node->input[1]->output.value.boolean;
	stream << "bool " << node->boxName() << " = " << node->input[0]->boxName() << "||" << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Logic::Not(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = !node->input[0]->output.value.boolean;
	stream << "bool " << node->boxName() << " = !" << node->input[0]->boxName() << ";\n";
}



void CFlat::BoxOperations::Attributes::getInteger(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_int;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.intValue);
	node->constOutput = false;

	//TODO hacer enteros
	stream << "int " << node->boxName() << " = " << node->input[0]->output.message << ";\n";
}

void CFlat::BoxOperations::Attributes::getDecimal(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_float;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.floatValue);
	node->constOutput = false;


	stream << "float " << node->boxName() << " = " << node->input[0]->output.message << ";\n";
}

void CFlat::BoxOperations::Attributes::getBoolean(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_boolean;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.boolean);
	node->constOutput = false;
	stream << "bool " << node->boxName() << " = " << node->input[0]->output.message << ";\n";
}

void CFlat::BoxOperations::Attributes::getString(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_string;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.message);
	node->constOutput = false;
	stream << "std::string " << node->boxName() << " = " << node->input[0]->output.message << ";\n";
}



void CFlat::BoxOperations::Attributes::setInteger(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.intValue
	);
	stream << node->input[0]->output.message << " = " << node->input[1]->boxName() << ";\n";
}

void CFlat::BoxOperations::Attributes::setDecimal(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.floatValue
	);

	stream << node->input[0]->output.message << " = " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Attributes::setBoolean(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.boolean
	);
	stream << node->input[0]->output.message << " = " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Attributes::setString(IBox* node, std::ofstream& stream) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.message
	);
	stream << node->input[0]->output.message << " = " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Math::Add(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue + node->input[1]->output.value.floatValue;
	stream << "float " << node->boxName() << " = " << node->input[0]->boxName() << " + " << node->input[1]->boxName() << ";\n";
}

void CFlat::BoxOperations::Math::Subtract(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue - node->input[1]->output.value.floatValue;
	stream << "float " << node->boxName() << " = " << node->input[0]->boxName() << " - " << node->input[1]->boxName() << ";\n";
}

void CFlat::BoxOperations::Math::Multiply(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue * node->input[1]->output.value.floatValue;
	stream << "float " << node->boxName() << " = " << node->input[0]->boxName() << " * " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Math::Division(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue / node->input[1]->output.value.floatValue;
	stream << "float " << node->boxName() << " = " << node->input[0]->boxName() << " / " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Math::Power(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = powf(node->input[0]->output.value.floatValue, node->input[1]->output.value.floatValue);
	stream << "float " << node->boxName() << " = powf(" << node->input[0]->boxName() << ", " << node->input[1]->boxName() << ");\n";
}



void CFlat::BoxOperations::Math::SquareRoot(IBox* node, std::ofstream& stream)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = sqrtf(node->input[0]->output.value.floatValue);

	stream << "float " << node->boxName() << " = sqrtf(" << node->input[0]->boxName() << ");\n";
}



void CFlat::BoxOperations::Math::Mod(IBox* node, std::ofstream& stream)
{
	//TODO el mod esta como float
	node->output.type = ParamType::_float;
	node->output.value.floatValue = (int)node->input[0]->output.value.floatValue % (int)node->input[1]->output.value.floatValue;
	stream << "int " << node->boxName() << " = " << node->input[0]->boxName() << " % " << node->input[1]->boxName() << ";\n";
}


void CFlat::BoxOperations::Math::PlusOne(IBox* node, std::ofstream& stream)
{
	if (node->input[0]->output.type == ParamType::_float) {
		node->output.type = ParamType::_float;
		node->output.value.floatValue = node->input[0]->output.value.floatValue + 1;
	}

	else if (node->input[0]->output.type == ParamType::_int) {
		node->output.type = ParamType::_int;
		node->output.value.intValue = node->input[0]->output.value.intValue + 1;
	}

	stream << "int " << node->boxName() << " = " << node->input[0]->boxName() << " + 1" << ";\n";
}


