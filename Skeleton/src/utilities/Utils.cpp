#include "Utils.h"

#include <random>

#define PI 3.14159265f

namespace Utilities {

	float Utils::DegreesToRadians(float angle) {

		return angle * PI / 180.0f;

	}

	float Utils::RadiansToDegrees(float angle) {

		return angle * 180.0f / PI;
	}

	float Utils::RandomBetween(float min, float max) {
		float r = rand() / (float)RAND_MAX;
		return r * (max - min) + min;
	}

	float Utils::RandomAngleBetween(float min, float max) {
		float degree = Utils::RandomBetween(min, max);

		return DegreesToRadians(degree);
	}

	int Utils::RandomIntBetween(int min, int max) {
		return min + std::rand() % (max - min + 1);
	}

	Color Utils::RandomColor() {
		Color c;
		
		c.r = RandomIntBetween(0, 255);
		c.g = RandomIntBetween(0, 255);
		c.b = RandomIntBetween(0, 255);

		return c;
	}

	Color Utils::RandomColorBetween(const Color& c1, const Color& c2) {
		Color c;

		c.r = RandomIntBetween(c1.r, c2.r);
		c.g = RandomIntBetween(c1.g, c2.g);
		c.b = RandomIntBetween(c1.b, c2.b);

		return c;
	}

}
