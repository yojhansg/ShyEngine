#include "EngineTime.h"

namespace Utilities {

	EngineTime::EngineTime() {
		deltaTime = 1.0f / 60.0f;
		fixedDeltaTime = 1.0f / 50.0f;
		timeSinceStart = 0;
		frames = 0;
	}

	float EngineTime::calculateFrameRate() {
		return frames / timeSinceStart * 1000.0f;
	}

}