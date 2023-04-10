#pragma once

#include "Singleton.h"
#include <string>
#include "EditorExport.h"

namespace Utilities {

	EditorManager Time : public Utilities::Singleton<Time> {

		friend Singleton<Time>;


	publish:
		float GetTimeSinceBegining();
		float GetTimeSinceBeginingMilliseconds();
		float GetDeltaTime();
		float GetFrameRate();
		float GetPhysicsDeltaTime();
		float ScaleWithDeltaTime(float val);
		float ScaleWithPhysicsDeltaTime(float val);
	public:

		float calculateFrameRate();

		float deltaTime;
		float fixedDeltaTime;
		float timeSinceStart;
		unsigned long long frames;

		static std::string Time2String(float time);

	private:

		Time();
	};
}