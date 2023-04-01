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


		void AddElement(OverlayElement* elem);
		void RemoveElement(OverlayElement* elem);

		void SetDirty();

	private:

		bool isDirty;

		std::vector<OverlayElement*> overlays;
	};

}

