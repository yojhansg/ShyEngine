#pragma once

#include <ostream>
#include <string>

namespace Utilities {

	class Color {

	public:

		static Color CreateColor(float r, float g, float b);
		static Color CreateColor(std::string const& str);
		static Color Lerp(const Color& startColor, const Color& endColor, float t);

		float r, g, b;

		static Color Red();
		static Color Green();
		static Color Blue();
		static Color Yellow();
		static Color Purple();
		static Color Orange();
		static Color Black();
		static Color White();
		static Color Grey();
		static Color DarkGrey();

		operator std::string() const;

	};

	// Needed for printing a value of tyep Vector2D with std::cout.
	// The definition is in .cpp
	std::ostream& operator<<(std::ostream& os, const Color& v);

	// Mul operator
	Color operator*(const Color& v, int n);

}

using cColor = Utilities::Color const&;