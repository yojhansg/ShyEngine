#pragma once

#include "Node.h"
#include "Scripting/Variable.h"

namespace Scripting {


	class OutputNode: public Node {

	protected:
		OutputNode(int idx);

	public:

		Scripting::Variable output;
	};

}