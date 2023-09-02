#pragma once

#include "OutputNode.h"
#include "Variable.h"

#include <string>

namespace Scripting {


	class ConstNode : public OutputNode{

	public:

		ConstNode(int idx, Variable val);

		void Operate(Node*& next, int& iterationIdx) override;

		void SetName(cstring name);

	private:

		std::string name;
	};

}