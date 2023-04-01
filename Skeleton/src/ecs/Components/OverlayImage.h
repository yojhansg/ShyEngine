#pragma once

#include <string>
#include <Component.h>

namespace Renderer {
	class Texture;
}

namespace ECS {


	class OverlayElement;
	class OverlayImage : public Component {

	public:

		OverlayImage();


		//TODO: path privado
		reflect std::string path;
	private:


		OverlayElement* overlay;

		Renderer::Texture* texture;

		void start() override;
		void render() override;

	publish:

		void SetTexture(std::string texture);
		std::string GetTexture();

	};

}