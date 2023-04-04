#pragma once

#include "Component.h"
#include "Color.h"

namespace ECS {



	class OverlayElement;
	class OverlayButton : public Component {



	private:

		reflect Utilities::Color defaultColor;
		reflect Utilities::Color hoverColor;
		reflect Utilities::Color downColor;


	};
}