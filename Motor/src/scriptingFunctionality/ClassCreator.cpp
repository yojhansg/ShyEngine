#include "ClassCreator.h"

ClassCreator::ClassCreator(std::string className, bool pragmaOnce, bool isSingleton) : className(className), isSingleton(isSingleton)
{
	addConstructor = addDestructor = false;

	if (pragmaOnce)
		classStream << "#pragma once\n\n";
	if (isSingleton)
		IncludeRelative("Singleton.h");
}

ClassCreator& ClassCreator::IncludeRelative(cstring path)
{
	classStream << "#include \"" << path << "\"\n";

	return *this;
}

ClassCreator& ClassCreator::IncludeAbsolute(cstring path)
{
	classStream << "#include <" << path << ">\n";

	return *this;
}

ClassCreator& ClassCreator::IncludeRelatives(std::vector<std::string> const& paths)
{
	for (auto const& p : paths) {
		IncludeRelative(p);
	}
	return *this;
}

ClassCreator& ClassCreator::IncludeAbsolutes(std::vector<std::string> const& paths)
{
	for (auto const& p : paths) {
		IncludeAbsolute(p);
	}
	return *this;
}

ClassCreator& ClassCreator::BeginClass()
{
	if (isSingleton) {

		classStream << "class " << className << ": public Utilities::Singleton<" << className << ">{\n";

		classStream << "\n";

		classStream << "\t friend Singleton<" + className << ">;\n";
	}
	else
		classStream << "class " << className << "{\n";

	return *this;
}

ClassCreator& ClassCreator::AddMethod(cstring returnType, cstring methodName, std::vector<Param> const& param, cstring content, bool isStatic)
{
	return AddMethod({ returnType, methodName, content, param, isStatic });
}


ClassCreator& ClassCreator::AddMethod(Method m)
{
	classStream << "\t";

	if (m.isStatic)
		classStream << "static ";

	classStream << m.returnType << " " << m.methodName << "(";

	for (Param const& p : m.params) {

		classStream << p.returnType << " " << p.name << ", ";
	}

	if (m.params.size() > 0)
		classStream.seekp(-2, classStream.cur);

	classStream << ");\n";

	methods.push_back(m);

	return *this;
}


ClassCreator& ClassCreator::AddAtribute(cstring type, cstring value)
{
	classStream << "\t" << type << " " << value << ";\n";
	return *this;
}


ClassCreator& ClassCreator::Public()
{
	classStream << "public: \n";
	return *this;
}


ClassCreator& ClassCreator::Private()
{
	classStream << "private: \n";
	return *this;
}

ClassCreator& ClassCreator::EndClass()
{
	classStream << "};\n";
	return *this;
}


ClassCreator& ClassCreator::AddComment(cstring comment)
{
	classStream << "//" << comment << "\n";
	return *this;
}

ClassCreator& ClassCreator::AddLine(cstring line)
{
	classStream << line << "\n";
	return *this;
}

ClassCreator& ClassCreator::AddDefine(cstring name, cstring value)
{
	classStream << "#define " << name << " " << value << "\n";
	return *this;
}

ClassCreator& ClassCreator::Empty(int lineCount)
{
	for (int i = 0; i < lineCount; i++) {

		classStream << "\n";
	}

	return *this;
}

ClassCreator& ClassCreator::AddCppInclude(std::string const& inc)
{
	includeCpp.push_back(inc);
	return *this;
}

ClassCreator& ClassCreator::AddConstructor(std::string const& constructorContent)
{
	addConstructor = true;
	constructor = constructorContent;

	classStream << "\t" << className << "();\n";

	return *this;
}

ClassCreator& ClassCreator::AddDestructor(std::string const& destructorContent)
{
	addDestructor = true;
	destructor = destructorContent;

	classStream << "\t~" << className << "();\n";

	return *this;
}

std::string ClassCreator::Header()
{
	return classStream.str();
}


std::string ClassCreator::Source()
{
	ClassCreator source(className, false);


	source.IncludeRelative(className + ".h");
	source.Empty();

	for (auto& inc : includeCpp) {

		source.IncludeRelative(inc);
	}
	source.Empty(3);

	if (addConstructor) {

		source.classStream << className << "::" << className << "(){\n";
		source.classStream << constructor << "\n}";
	}

	if (addDestructor) {
		source.classStream << className << "::~" << className << "(){\n";
		source.classStream << destructor << "\n}";
	}


	source.Empty(3);

	for (auto& m : methods) {

		source.AddMethod(m.returnType, className + "::" + m.methodName, m.params, "", false);

		source.classStream.seekp(-2, source.classStream.end);

		source.classStream << "{\n";

		source.classStream << m.content << "\n";

		source.classStream << "}\n";
	}

	return source.Header();
}