#include "Color.h"
#include <format>
#include <cassert>
#include "StringTrim.h"


Utilities::Color Utilities::Color::CreateColor(int r, int g, int b)
{
	Color color;

	color.r = r;
	color.g= g;
	color.b = b;
	return color;
}

Utilities::Color Utilities::Color::CreateColor(std::string const& str)
{
	std::string trimed = Utilities::trim(str, "([ )]");

	size_t coma = trimed.find(",");

	assert(coma != std::string::npos);

	std::string r_ = trimed.substr(0, coma);

	std::string secondPart = trimed.substr(coma + 1);

	coma = secondPart.find(",");

	std::string g_ = secondPart.substr(0, coma);
	std::string b_ = secondPart.substr(coma + 1);


	Utilities::Color color;
	color.r = std::stoi(r_);
	color.g = std::stoi(g_);
	color.b = std::stoi(b_);
	return color;
}

Utilities::Color Utilities::Color::Red()
{
	return CreateColor(255, 0, 0);
}

Utilities::Color Utilities::Color::Green()
{
	return CreateColor(255, 0, 0);
}

Utilities::Color Utilities::Color::Blue()
{
	return CreateColor(255, 0, 0);
}

Utilities::Color Utilities::Color::Yellow()
{
	return CreateColor(255, 255, 0);
}

Utilities::Color Utilities::Color::Purple()
{
	return CreateColor(255, 0, 255);
}

Utilities::Color Utilities::Color::Orange()
{
	return CreateColor(255, 165, 0);
}

Utilities::Color Utilities::Color::Black()
{
	return CreateColor(0, 0, 0);
}

Utilities::Color Utilities::Color::White()
{
	return CreateColor(255, 255, 255);
}

Utilities::Color::operator std::string() const {

	return std::format("({}, {}, {})", r, g, b);
}

std::ostream& Utilities::operator<<(std::ostream& os, const Color& v)
{
	os << (std::string) v;
	return os;
}
