#pragma once

#include "Singleton.h"
#include <vector>

namespace ECS {

	class Entity;
	class Overlay;
	class OverlayText;
	class OverlayManager : public Utilities::Singleton<OverlayManager> {

	public:

		friend class Engine;

		OverlayManager();
		OverlayManager(bool debugFrameRate, float timeToDoubleClick, float timeToHold);
		~OverlayManager();

		void Render();
		void Update();

		void AddElement(Overlay* elem);
		void RemoveElement(Overlay* elem);

		void SetDirty();
		bool IsDirty();

		void RecalculateOverlay();

	private:

		bool isDirty;

		//Input:
		//TODO: que esto vaya a la configuracion global
		float lastClickTime;
		float timeToDoubleClick;
		float timeToHold;
		float holdTimer;

		Entity* frameRateEntity;
		OverlayText* frameRateText;

		Overlay* selected;
		std::vector<Overlay*> overlays;
	};

}

