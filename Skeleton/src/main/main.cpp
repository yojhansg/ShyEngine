#include <memory>
#include <iostream>
#include "Engine.h"

#include "Scripting/Node.h"
#include "Scripting/ConstNode.h"
#include "Scripting/Function.h"
#include "EditorExport.h"

using namespace Scripting;


/*

	TODO:

	Como vamos a serializar los objetos de la escena
	y como podemos conseguir las referencias a ellos luego en el codigo

*/



int main(int argc, char* args[]) {

	//TODO: quitar esto y quitar tambien los includes que he ido dejando por ahi

	Function* suma = new Function(0, 0, "");

	ConstNode* dos = new ConstNode(0, 2);
	ConstNode* tres = new ConstNode(0, 3);


	suma->SetInput({ dos, tres });


	Node* inicial = suma;

	inicial->Cicle();


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	Engine engine;

	engine.init();

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";
	
	return 0;
}