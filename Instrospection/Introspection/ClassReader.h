#pragma once


#define publish public

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define WHITESPACE " \n\r\t\f\v"

//Limitado a un componente por .h
class ClassReader {
private:


	static std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}


public:

	struct method {
		std::string output;
		std::string name;
		std::vector<std::pair<std::string, std::string>> params;
	};


	struct classDef {

		std::string className;
		std::vector<method> methods;
	};

	static std::vector<method> getMethods(std::string const& file) {

		std::ifstream stream(file);
		std::vector<method> methods;

		bool inPublish = false;
		while (stream) {

			std::string line;
			std::getline(stream, line);

			line = ltrim(line);

			if (line == "publish:") {
				inPublish = true;
			}
			else if (line == "" || line == "public:" || line == "private" || line == "protected:")
				inPublish = false;

			else if (inPublish) {

				method m = processMethod(line);
				methods.push_back(m);
			}
		}

		return methods;
	}
private:

	static method processMethod(std::string const& line) {

		method m;
		int idx = 0;
		std::string word = "";
		std::string wordAux = "";
		bool isAux = true;
		for (int i = 0; i < line.size(); i++) {

			char c = line[i];
			if (c == ' ' || c == '(' || c == ')') {

				switch (idx)
				{
				case 0:
					m.output = word;
					idx = 1;
					break;
				case 1:
					m.name = word;
					idx = 2;
					break;
				default:
					if (isAux) wordAux = word;
					else {

						m.params.push_back(std::make_pair(wordAux, word));
					}

					isAux = !isAux;
					break;
				}
				word = "";
				continue;
			}

			word += c;
		}
		return m;
	}
};