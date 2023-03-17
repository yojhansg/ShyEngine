#include "FlowManager.h"
#include <fstream>
#include "json.hpp"
#include <iostream>
#include "CFlatBasics.h"
#include "Script.h"

#include <fstream>

using nlohmann::json;
using std::string;
using jsonarray = std::vector<json>;


CFlat::FlowManager* CFlat::FlowManager::instance;

namespace CFlat {

	FlowManager::FlowManager()
	{
		instance = this;
	}
	FlowManager::ScriptInfo* FlowManager::getBoxes(std::string file)
	{
		return scripts.find(file)->second;
	}

	FlowManager::ScriptInfo* FlowManager::loadScript(string file, Script* script)
	{
		//TODO: separar este metodo que es un poco enorme en cachitos mas aceptables

		//Antes de inicializar el script comprobamos que no exista de antes
		{
			auto search = scripts.find(file);
			if (search != scripts.end()) {
				return search->second;
			}
		}

		//TODO manejo de error -> fichero script no existe
		//Si el archivo no existe retornamos y se hace el manejo de errores
		std::ifstream f(path + file + fileExtension);
		if (!f.good()) {

			std::cout << "El archivo no existe";
			return nullptr;
		}
		std::ofstream output("Hola.txt");
		output << "#pragma once\n#include<iostream>\n#include<string>\n\n";

		json data = json::parse(f);

		int boxCount = data["nodeCount"].get<int>();

		boxes = std::vector<CFlat::IBox*>(boxCount);

		/*
		* Vectores temporales con todas los inputs, enlaces y divisiones
		* Una vez se hayan creado todas las cajas se establecen los punteros de las cajas
		**/

		std::vector<std::vector<int>> boxInput(boxCount);
		std::vector<std::pair<int, int>> boxLink;
		std::vector<std::pair<int, int>> boxSplit;

		/*
		* Nota:
		* El vector de input se procesa de forma diferente:
		* Es un vector de vectores, siendo el indice del primer vector el indice del vector al cual le corresponde un vector de input
		*/


		/*
		* Se dividen los nodos entre : input, operadores y logica, pues se leen y procesan de forma diferente
		*/


		/*
		* Input contiene todas las variables constantes del programa
		*/

		if (data.contains("input")) {

			jsonarray input = data["input"].get<jsonarray>();
			for (json in : input) {

				int id = in["id"].get<int>();

				switch (in["type"].get<std::string>()[0]) {

				case 'f':

					boxes[id] = CFlat::Attributes::createFloat(in["value"].get<float>(), id);
					break;

				case 'i':

					boxes[id] = CFlat::Attributes::createInt(in["value"].get<int>(), id);
					break;

				case 's':

					boxes[id] = CFlat::Attributes::createString(in["value"].get<std::string>(), id);
					break;
				}
			}
		}

		/*
		*  Operators contiene todas las operaciones y funcionalidad del codigo
		*/
		if (data.contains("operator")) {
			jsonarray operators = data["operator"].get<jsonarray>();

			for (json op : operators) {

				int id = op["id"].get<int>();

				int type = op["type"].get<int>();

				if (op.contains("next")) {
					int nextBox = op["next"].get<int>();

					boxLink.push_back(std::make_pair(id, nextBox));
				}

				if (op.contains("input")) {
					jsonarray operatorInput = op["input"].get<jsonarray>();
					for (json oi : operatorInput)
						boxInput[id].push_back(oi.get<int>());
				}

				boxes[id] = new CFlat::IBox(type, id);
			}
		}

		/*
		*  La logica consiste en condicionales y bucles
		*/
		if (data.contains("logic")) {
			jsonarray logic = data["logic"].get<jsonarray>();


			for (json cond : logic) {

				int id = cond["id"].get<int>();

				int input = cond["input"].get<int>();
				boxInput[id].push_back(input);

				int type = cond["type"].get<int>();

				if (cond.contains("next"))
					boxLink.push_back(std::make_pair(id, cond["next"].get<int>()));

				int alternativeBox = cond.contains("alter") ? cond["alter"].get<int>() : -1;
				boxSplit.push_back(std::make_pair(id, alternativeBox));
				boxes[id] = new CFlat::Split(type, id);
			}
		}
		/*
		*	Atributos y variables del script
		*/
		if (data.contains("attributes")) {

			json attributes = data["attributes"].get<json>();

			
			if (attributes.contains("integer")) {
				jsonarray integer = attributes["integer"].get<jsonarray>();

				for (json i : integer) {

					std::string name = i["name"].get<std::string>();
					int value = i["value"].get<int>();

					output << "int " << name << " = " << std::to_string(value) << ";\n";
					script->attributes.generateAttribute(name, value);
				}
			}

			if (attributes.contains("decimal")) {
				jsonarray decimals = attributes["decimal"].get<jsonarray>();

				for (json i : decimals) {

					std::string name = i["name"].get<std::string>();
					float value = i["value"].get<float>();

					output << "float " << name << " = " << std::to_string(value) << ";\n";
					script->attributes.generateAttribute(name, value);
				}
			}

			if (attributes.contains("boolean")) {
				jsonarray booleans = attributes["boolean"].get<jsonarray>();

				for (json i : booleans) {
					
					std::string name = i["name"].get<std::string>();
					bool value = i["value"].get<bool>();

					output << "bool " << name << " = " << std::to_string(value) << ";\n";

					script->attributes.generateAttribute(name, value);
				}
			}

			if (attributes.contains("string")) {
				jsonarray integer = attributes["string"].get<jsonarray>();

				for (json i : integer) {

					std::string name = i["name"].get<std::string>();
					std::string value = i["value"].get<std::string>();

					output << "bool " << name << " = \"" << value << "\";\n";

					script->attributes.generateAttribute(name, value);
				}
			}
		}

		/*
		*  Se establecen todos los inputs
		*/
		for (int i = 0; i < boxCount; i++) {

			for (int c = 0; c < boxInput[i].size(); c++) {
				CFlat::IBox* inputBox = boxes[boxInput[i][c]];
				boxes[i]->addInput(inputBox);
			}
		}

		//Links
		for (std::pair<int, int> pair : boxLink) {
			if (pair.second >= 0)
				boxes[pair.first]->nextBox = boxes[pair.second];
		}

		//Splits
		for (std::pair<int, int> pair : boxSplit) {

			CFlat::Split* split = static_cast<CFlat::Split*>(boxes[pair.first]);
			CFlat::IBox* alternativeBox = pair.second >= 0 ? boxes[pair.second] : nullptr;
			split->setOtherRoute(alternativeBox);
		}



		//TODO: cada nodo tiene que tener su id (para facilitar la depuracion pero para la deteccion de errores)
		//Establecer a cada nodo una referencia al script
		for (CFlat::IBox* box : boxes) {

			if (box != nullptr)
				box->script = script;
		}


		//TODO: en vez de crear un script info, rellenar el script aqui dentro
		/*
		*  Lectura de los puntos de entrada en el programa
		*/

		if (data.contains("init")) {
			int initid = data["init"].get<int>();
			if (initid >= 0)
				script->initBox = boxes[initid];
		}

		if (data.contains("update")) {
			int updateid = data["update"].get<int>();
			if (updateid >= 0)
				script->updateBox = boxes[updateid];
		}
	

		//scripts.emplace(file, scriptInfo);
		output << "int main(){\n\n";


		script->init(output);

		output << "}";
		output.close();
		return nullptr;
	}

}