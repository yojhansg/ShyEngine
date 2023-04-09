#pragma once

#include <string>
#include <Component.h>

namespace Renderer {
	class Texture;
}

namespace ECS {


	class Overlay;
	class OverlayImage : public Component {

	public:

		OverlayImage();


		reflect std::string path;
	private:


		Overlay* overlay;

		Renderer::Texture* texture;

		void init() override;
		void render() override;

	publish:

		void SetTexture(std::string texture);
		std::string GetTexture();

	};

}