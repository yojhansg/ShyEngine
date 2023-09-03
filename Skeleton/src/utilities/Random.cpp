#include "Random.h"

#include <random>

#define PI 3.14159265f

namespace Utilities {

	int Random::RandomIntBetween(int min, int max) {
		return min + std::rand() % (max - min + 1);
	}

	float Random::RandomBetween(float min, float max) {
		float r = rand() / (float) RAND_MAX;
		return r * (max - min) + min;
	}

	float Random::RandomAngleBetween(float min, float max) {
		float degree = Random::RandomBetween(min, max);

		return degree * PI / 180.0f;
	}

	Color Random::RandomColor() {
		Color c;
		
		c.r = RandomIntBetween(0, 255);
		c.g = RandomIntBetween(0, 255);
		c.b = RandomIntBetween(0, 255);

		return c;
	}

	Color Random::RandomColorBetween(cColor c1, cColor c2) {
		Color c;

		c.r = RandomIntBetween(c1.r, c2.r);
		c.g = RandomIntBetween(c1.g, c2.g);
		c.b = RandomIntBetween(c1.b, c2.b);

		return c;
	}

}
