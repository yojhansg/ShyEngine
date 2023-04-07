#pragma once

#include "OutputNode.h"
#include "Variable.h"

namespace Scripting {


	class ConstNode : public OutputNode{

	public:

		ConstNode(int idx, Variable val);

		void Operate(Node*& next, int iterationIdx) override;

	private:

		Variable value;
	};

}