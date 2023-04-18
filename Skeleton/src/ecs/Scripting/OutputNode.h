#pragma once

#include "Node.h"
#include "Scripting/Variable.h"

namespace Scripting {


	class OutputNode: public Node {

	public:

		OutputNode(int idx);


		Scripting::Variable output;
		Scripting::Variable originalValue;
	};

}