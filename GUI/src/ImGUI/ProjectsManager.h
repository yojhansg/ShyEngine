#pragma once

#include "Window.h"

class ImGUIManager;

namespace PEditor {

	class ProjectsManager : public Window {

	public:

		ProjectsManager();

		void update() override;
		void handleInput(SDL_Event* event) override;
		void render() override;

	private:

		ImGUIManager* imGuiManager;

	};

}

