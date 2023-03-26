#pragma once

#include "OutputNode.h"
#include <vector>
#include <string>

//TODO: cambiar nombre de function a functionNode para mayor claridad

namespace Scripting {



	class Function: public OutputNode {


	public:

		enum class Operation {

			FunctionCall, Print, 

			//Math operations
			Add, Subtract, Multiply, Division, Power, SquareRoot, Mod, PlusOne,

			//Logic operations
			Equal, Different, Minor, Greater, MinorEqual, GreaterEqual, And, Or, Not,

			OperationCount
		};


		Function(int idx, std::string const& method = "");

		void Operate(Node*& nextNode) override;


		void SetNextNode(Node* nextNode);

		void SetInput(std::vector<OutputNode*> input);

	private:


		Node* next;
		std::string methodName;

		std::vector<OutputNode*> input;
	};

}