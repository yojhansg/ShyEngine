#pragma once

#include <string>
#include <vector>
#include <sstream>

using cstring = std::string const&;

class ClassCreator {

public:

	struct Param {
		std::string returnType;
		std::string name;
	};

	struct Method {

		std::string returnType;
		std::string methodName;
		std::string content;
		std::vector<Param> params;

		bool isStatic = false;
	};

	ClassCreator(std::string className, bool pramaOnce = true);

	ClassCreator& BeginClass();
	ClassCreator& EndClass();

	ClassCreator& IncludeRelative(cstring path);
	ClassCreator& IncludeAbsolute(cstring path);
	ClassCreator& IncludeRelatives(std::vector<std::string> const& path);
	ClassCreator& IncludeAbsolutes(std::vector<std::string> const& paths);
	
	ClassCreator& Public();
	ClassCreator& Private();

	ClassCreator& AddAtribute(cstring type, cstring value);
	ClassCreator& AddMethod(Method method);
	ClassCreator& AddMethod(cstring returnType, cstring methodName, std::vector<Param> const& param = {},
		cstring methodContent = "", bool isStatic = false);

	ClassCreator& AddDefine(cstring name, cstring value = "");
	ClassCreator& AddComment(cstring comment);

	ClassCreator& AddLine(cstring line);
	ClassCreator& Empty(int lineCount = 1);

	std::string Header();
	std::string Source();

private:

	std::string className;
	std::stringstream classStream;
	std::vector<Method> methods;
};