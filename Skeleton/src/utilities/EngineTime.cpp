#include "EngineTime.h"

namespace Utilities {

	EngineTime::EngineTime() {
		deltaTime = 1.0f / 144.0f;
		timeSinceStart = 0;
		frames = 0;
	}

	float EngineTime::calculateFrameRate() {
		return frames / timeSinceStart * 1000.0f;
	}

}