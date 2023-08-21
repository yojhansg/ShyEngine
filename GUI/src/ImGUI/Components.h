#pragma once
#include <vector>
#include "Window.h"

#define COMPONENTS_WIN_WIDTH_RATIO 0.4f
#define COMPONENTS_WIN_HEIGHT_RATIO 0.7f

namespace ShyEditor {

	class ComponentWindow : public Window {

	private:

	public:

		ComponentWindow();

		void Behaviour() override;

	};
}
