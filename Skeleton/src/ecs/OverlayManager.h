#pragma once

#include "Singleton.h"
#include <vector>

namespace ECS {

	class Entity;
	class OverlayElement;
	class OverlayManager : public Utilities::Singleton<OverlayManager> {

	public:

		friend class Engine;

		OverlayManager();
		~OverlayManager();

		void Render();
		void Update(float dt);

		void AddElement(OverlayElement* elem);
		void RemoveElement(OverlayElement* elem);

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


		OverlayElement* selected;
		std::vector<OverlayElement*> overlays;
	};

}

