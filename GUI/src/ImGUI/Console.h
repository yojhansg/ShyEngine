#pragma once

#include "Window.h"
#include <string>

namespace ShyEditor {

	class Console : public Window {

	private:

		struct Message {

			struct Color {
				int r, g, b;
			};

			Color color;

			std::string date;
			std::string type;
			std::string message;
		};



		std::vector<Message> messages;

	public:

		Console();
		void Behaviour() override;
	};

}