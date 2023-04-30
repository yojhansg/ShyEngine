#pragma once

#include "Color.h"

namespace Utilities {

	class Utils {

	public:

		// Convierte un angulo de grados a radianes
		static float DegreesToRadians(float angle);

		// Convierte un angulo de radianes a grados
		static float RadiansToDegrees(float angle);

		// Genera un numero aleatorio entre min y max
		static float RandomBetween(float min, float max);

		// Genera un angulo aleatorio entre dos dados (en grados)
		static float RandomAngleBetween(float min, float max);

		// Genera un entero aleatorio entre min y max
		static int RandomIntBetween(int min, int max);

		// Genera un color aleatorio
		static Color RandomColor();

		// Genera un color aleatorio entre otros dados
		static Color RandomColorBetween(const Color& c1, const Color& c2);

	};

}

