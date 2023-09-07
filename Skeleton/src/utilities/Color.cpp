#include "Color.h"
#include <format>
#include <cassert>
#include "StringTrim.h"
#include <map>

namespace Utilities {

	Color Color::CreateColor(float r, float g, float b)
	{
		Color color;

		color.r = r;
		color.g = g;
		color.b = b;
		return color;
	}

	Color Color::CreateColor(std::string const& str)
	{
		//Temporal: mapa de colores para no poner los colores a mano en los scripts:
		std::map<std::string, Color> colores;

		colores["red"] = Red();
		colores["green"] = Green();
		colores["blue"] = Blue();
		colores["yellow"] = Yellow();
		colores["purple"] = Purple();
		colores["orange"] = Orange();
		colores["black"] = Black();
		colores["white"] = White();
		colores["grey"] = Grey();
		colores["darkGrey"] = DarkGrey();

		if (colores.contains(str))
			return colores[str];


		std::string trimed = trim(str, "([ )]");

		size_t coma = trimed.find(",");

		assert(coma != std::string::npos);

		std::string r_ = trimed.substr(0, coma);

		std::string secondPart = trimed.substr(coma + 1);

		coma = secondPart.find(",");

		std::string g_ = secondPart.substr(0, coma);
		std::string b_ = secondPart.substr(coma + 1);


		Color color;
		color.r = std::stof(r_);
		color.g = std::stof(g_);
		color.b = std::stof(b_);
		return color;
	}

	Color Color::Lerp(const Color& startColor, const Color& endColor, float t) {
		int r = startColor.r * (1 - t) + endColor.r * t;
		int g = startColor.g * (1 - t) + endColor.g * t;
		int b = startColor.b * (1 - t) + endColor.b * t;
		return Color(r, g, b);
	}

	Color Color::Red()
	{
		return CreateColor(255, 0, 0);
	}


	Color Color::Green()
	{
		return CreateColor(255, 0, 0);
	}

	Color Color::Blue()
	{
		return CreateColor(255, 0, 0);
	}

	Color Color::Yellow()
	{
		return CreateColor(255, 255, 0);
	}

	Color Color::Purple()
	{
		return CreateColor(255, 0, 255);
	}

	Color Color::Orange()
	{
		return CreateColor(255, 165, 0);
	}

	Color Color::Black()
	{
		return CreateColor(0, 0, 0);
	}

	Color Color::White()
	{
		return CreateColor(255, 255, 255);
	}

	Color Color::Grey()
	{
		return CreateColor(200, 200, 200);
	}

	Color Color::DarkGrey()
	{
		return CreateColor(51, 51, 51);
	}

	Color::operator std::string() const {

		return std::format("({}, {}, {})", r, g, b);
	}

	std::ostream& operator<<(std::ostream& os, const Color& v)
	{
		os << (std::string)v;
		return os;
	}

	Color operator*(const Color& v, int n) {
		int R = v.r * n;
		int G = v.g * n;
		int B = v.b * n;

		return Color(R, G, B);
	}

}
