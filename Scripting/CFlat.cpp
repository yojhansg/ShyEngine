#include "CFlat.h"
#include "OperatorManager.h"

namespace CFlat {


	IBox::IBox(int op, int id) {

		operation = op;
		nextBox = nullptr;
		constOutput = true;

		boxId = id;
		output.type = ParamType::_null;

		constValueOperation = "";

		used = false;
	}

	IBox::~IBox()
	{
		for (IBox* in : input) {
			delete in;
		}
		input.clear();
		nextBox = nullptr;
	}

	void IBox::validateInput(std::ofstream& stream)
	{
		for (IBox* box : input) {

			/*	TODO:
				
				*Estas comprobaciones tienen que mejorarse para asegurarse que funcione correctamente
				* 
				* Distinguir cuando se este ejecutando para generar una build o cuando se use en ejecucion
			*/
			if (!box->constOutput || box->output.type == ParamType::_null) //!box->used//)
			{
				box->processBox(stream);
				constOutput &= box->constOutput;
			}
		}
	}

	std::string IBox::boxName()
	{
		return "IBox_" + std::to_string(boxId);
	}

	void IBox::processBox(std::ofstream& stream)
	{
		validateInput(stream);
		OperatorManager::ProccessOperation(operation, this, stream);
		used = true;
	}

	void IBox::addInput(IBox* newInput) {
		input.push_back(newInput);
	}
}