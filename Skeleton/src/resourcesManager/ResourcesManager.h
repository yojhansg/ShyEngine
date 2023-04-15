#pragma once

#include <Singleton.h>
#include <unordered_map>
#include <string>

namespace Renderer {
	class Texture;
	class Font;
}

namespace Sound {
	class SoundEffect;
	class Music;
}

namespace Resources {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class ResourcesManager : public Utilities::Singleton<ResourcesManager> {

		friend Singleton<ResourcesManager>;

	public:

		~ResourcesManager();

		Renderer::Texture* addTexture(const std::string& key);
		Renderer::Font* addFont(std::string const& key, int pointSize);
		Sound::SoundEffect* addSound(std::string const& key);
		Sound::Music* addMusic(std::string const& key);

	private:

		ResourcesManager();
		
		resources_map<Renderer::Texture*> textures;
		resources_map<Renderer::Font*> fonts;
		resources_map<Sound::SoundEffect*> sounds;
		resources_map<Sound::Music*> music;
	};
}

