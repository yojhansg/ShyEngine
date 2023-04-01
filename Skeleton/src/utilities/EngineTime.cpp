#include "EngineTime.h"
#include <sstream>
#include <iomanip>

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


	std::string EngineTime::Time2String(float time) {


		float whole;

		int mill = std::modf(time, &whole) * 1000;

		int min = whole / 60;

		int sec = (int)whole % 60;


		std::stringstream stream;

		stream << std::setw(2) << std::setfill('0') << min << ":";
		stream << std::setw(2) << std::setfill('0') << sec << ":";
		stream << std::setw(3) << std::setfill('0') << mill;

		return stream.str();
	}

}