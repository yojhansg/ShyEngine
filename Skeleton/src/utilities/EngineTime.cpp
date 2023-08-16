#include "EngineTime.h"
#include <sstream>
#include <iomanip>

namespace Utilities {

	Time::Time() {
		deltaTime = 1.0f / 60.0f;
		fixedDeltaTime = 1.0f / 50.0f;
		timeSinceStart = 0;
		frames = 0;
	}

	float Time::calculateFrameRate() {
		return frames / timeSinceStart * 1000.0f;
	}

	float Time::GetTimeSinceBegining() {
		return timeSinceStart * 0.001;
	}

	float Time::GetTimeSinceBeginingMilliseconds() {
		return timeSinceStart;
	}

	float Time::GetFrameRate() {
		return calculateFrameRate();
	}

	float Time::GetDeltaTime() {
		return deltaTime;
	}

	float Time::GetPhysicsDeltaTime() {
		return fixedDeltaTime;
	}

	std::string Time::Time2String(float time) {

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

	float Time::ScaleWithDeltaTime(float val) {
		return val * GetDeltaTime();
	}

	float Time::ScaleWithPhysicsDeltaTime(float val) {
		return val * GetPhysicsDeltaTime();
	}

}