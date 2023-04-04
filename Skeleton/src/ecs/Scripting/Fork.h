#pragma once

#include "Node.h"

namespace Scripting {

	//TODO: cambiar nomrbe de fork a forkNode para mayor claridad

	class OutputNode;

	class Fork: public Node {

	public:
		enum class ForkType {

			If, While, For
		};

		Fork(int idx, ForkType type);

		void SetFork(Node* A, Node* B);
		void SetCondition(OutputNode* node);
		void Operate(Node*& next) override;

	private:


		Node* A;
		Node* B;

		OutputNode* condition;

		ForkType type;
	};
}