#pragma once

#include "Singleton.h"

namespace Utilities {

	class EngineTime : public Utilities::Singleton<EngineTime> {

		friend Singleton<EngineTime>;

	public:

		float calculateFrameRate();

		float deltaTime;
		float fixedDeltaTime;
		float timeSinceStart;
		unsigned long long frames;

	private:

		EngineTime();
	};
}