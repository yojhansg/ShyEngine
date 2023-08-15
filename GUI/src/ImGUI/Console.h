#pragma once

#include "Window.h"

namespace PEditor {



	class Console : public Window {


	private:
		bool close;


	public:

		Console();

		virtual void render();

	};

}