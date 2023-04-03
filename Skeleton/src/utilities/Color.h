#pragma once

#include <ostream>
#include <string>

namespace Utilities {

	class Color {

	public:

		Color();
		Color(int r, int g, int b);
		Color(std::string const& str);

		int r, g, b;

		static Color Red();
		static Color Green();
		static Color Blue();
		static Color Yellow();
		static Color Purple();
		static Color Orange();
		static Color Black();
		static Color White();

		operator std::string() const;
	};

	// needed for printing a value of tyep Vector2D with std::cout.
	// The definition is in .cpp
	std::ostream& operator<<(std::ostream& os, const Color& v);

}
