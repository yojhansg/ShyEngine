#pragma once

#include "Singleton.h"
#include <string>

namespace Utilities {

	class EngineTime : public Utilities::Singleton<EngineTime> {

		friend Singleton<EngineTime>;

	public:

		float calculateFrameRate();

		float deltaTime;
		float fixedDeltaTime;
		float timeSinceStart;
		unsigned long long frames;

		static std::string Time2String(float time);

	private:

		EngineTime();
	};
}