#pragma once

#include "Component.h"
#include "Color.h"
#include "Vector2D.h"

namespace ECS {



	class Overlay;
	class OverlayButton : public Component {

	public:

		OverlayButton();
	private:

		Overlay* overlay;

		reflect Utilities::Color defaultColor;
		reflect Utilities::Color hoverColor;
		reflect Utilities::Color downColor;

		reflect float scale;


		void init() override;
		void start() override;
		void onMouseEnter() override;
		void onMouseExit() override;
		void onClickBegin() override;
		void onClickHold() override;
		void onClick() override;
	};
}