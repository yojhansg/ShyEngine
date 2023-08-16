#pragma once

#include "Window.h"
#include <string>


namespace PEditor {


	class Console : public Window {

		struct Message {

			struct Color {
				int r, g, b;
			};

			Color color;

			std::string date;
			std::string type;
			std::string message;
		};


	private:
		bool visible;
		bool gameOpenedLastFrame;

		static bool forceOpen;

		std::vector<Message> messages;

	public:

		Console();

		static void ForceOpen();

		virtual void render();

	};

}