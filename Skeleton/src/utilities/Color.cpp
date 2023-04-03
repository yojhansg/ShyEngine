#include "Color.h"
#include <format>
#include <cassert>
#include "StringTrim.h"

Utilities::Color::Color()
{
	r = g = b = 255;
}

Utilities::Color::Color(int r, int g, int b) : r(r), g(g), b(b)
{
}

Utilities::Color::Color(std::string const& str)
{

	std::string trimed = Utilities::trim(str, "([ )]");

	size_t coma = trimed.find(",");

	assert(coma != std::string::npos);

	std::string r_ = trimed.substr(0, coma);

	std::string secondPart = trimed.substr(coma + 1);

	coma = secondPart.find(",");

	std::string g_ = secondPart.substr(0, coma);
	std::string b_ = secondPart.substr(coma + 1);


	r = std::stoi(r_);
	g = std::stoi(g_);
	b = std::stoi(b_);
}

Utilities::Color Utilities::Color::Red()
{
	return Color();
}

Utilities::Color Utilities::Color::Green()
{
	return Color();
}

Utilities::Color Utilities::Color::Blue()
{
	return Color();
}

Utilities::Color Utilities::Color::Yellow()
{
	return Color();
}

Utilities::Color Utilities::Color::Purple()
{
	return Color();
}

Utilities::Color Utilities::Color::Orange()
{
	return Color();
}

Utilities::Color Utilities::Color::Black()
{
	return Color();
}

Utilities::Color Utilities::Color::White()
{
	return Color();
}

Utilities::Color::operator std::string() const {

	return std::format("({}, {}, {})", r, g, b);
}

std::ostream& Utilities::operator<<(std::ostream& os, const Color& v)
{
	os << (std::string) v;
	return os;
}
