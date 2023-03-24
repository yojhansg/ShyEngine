#include "ClassCreator.h"

ClassCreator::ClassCreator(std::string className, bool pragmaOnce): className(className)
{
	if (pragmaOnce)
		classStream << "#pragma once\n\n";
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

ClassCreator& ClassCreator::IncludeAbsolutes(std::vector<std::string> const&  paths)
{
	for (auto const& p : paths) {
		IncludeAbsolute(p);
	}
	return *this;
}

ClassCreator& ClassCreator::BeginClass()
{
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

std::string ClassCreator::Header()
{
	return classStream.str();
}


std::string ClassCreator::Source()
{
	ClassCreator source(className, false);


	source.classStream << "\t";
	source.IncludeRelative(className + ".h");
	source.Empty();

	for (auto& inc : includeCpp) {

		source.classStream << "\t";
		source.IncludeRelative(inc);
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