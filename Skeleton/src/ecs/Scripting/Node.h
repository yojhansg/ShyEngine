#pragma once

namespace Scripting {


	/*
		Jerarquia de clases:

			Node
				\
				 \----> OutputNode
				  \				\
				   \             \----> FunctionNode
				    \             \---> ConstNode
				     \
				      \---------------> ForkNode
	*/

	class Node {

	public:

		Node(int idx);
		virtual ~Node();

		virtual void Operate(Node*& nextNode, int iterationIdx) = 0;
		/*
			Procesa este nodo y todos sus nodos siguientes
		*/

		virtual void ForceReevaluation();

		void Cicle(int iterationIdx);

		int iterationIdx;
	protected:

		int nodeIdx;
	};

}