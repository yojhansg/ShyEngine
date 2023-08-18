#include "ResourcesManager.h"
#include <RendererManager.h>
#include <Texture.h>
#include <Font.h>
#include <SoundEffect.h>
#include <Music.h>

Resources::ResourcesManager::ResourcesManager() : resourcesPath("") {}

Resources::ResourcesManager::ResourcesManager(const std::string& path) {

	if (path == "")
		resourcesPath = "";
	else
		resourcesPath = path + "/";
}

Resources::ResourcesManager::~ResourcesManager() {

	for (auto it = textures.begin(); it != textures.end(); it++) delete it->second;
	textures.clear();

	for (auto it = fonts.begin(); it != fonts.end(); it++) delete it->second;
	fonts.clear();

	for (auto it = sounds.begin(); it != sounds.end(); it++) delete it->second;
	sounds.clear();

	for (auto it = music.begin(); it != music.end(); it++) delete it->second;
	music.clear();
}

Renderer::Texture* Resources::ResourcesManager::addTexture(const std::string& key) {

	if (textures.contains(key))
		return textures.at(key);

	Renderer::Texture* t = new Renderer::Texture(resourcesPath + "Images/" + key);

	textures.insert(std::make_pair(key, t));

	return t;

}

Renderer::Font* Resources::ResourcesManager::addFont(std::string const& key, int pointSize) {

	std::string newkey = key + std::to_string(pointSize);

	if (fonts.contains(newkey))
		return fonts.at(newkey);

	Renderer::Font* t = new Renderer::Font(resourcesPath + "Fonts/" + key, pointSize);

	fonts.insert(std::make_pair(newkey, t));

	return t;
}

Sound::SoundEffect* Resources::ResourcesManager::addSound(std::string const& key) {

	if (sounds.contains(key))
		return sounds.at(key);

	Sound::SoundEffect* s = new Sound::SoundEffect(resourcesPath + "Sounds/" + key);

	sounds.insert(std::make_pair(key, s));

	return s;

}

Sound::Music* Resources::ResourcesManager::addMusic(std::string const& key) {

	if (music.contains(key))
		return music.at(key);

	Sound::Music* m = new Sound::Music(resourcesPath + "Sounds/" + key);

	music.insert(std::make_pair(key, m));

	return m;

}

std::string Resources::ResourcesManager::GetResourcesPath()
{
	return instance()->resourcesPath;
}

void Resources::ResourcesManager::SetResourcesPath(const std::string& path)
{
	instance()->resourcesPath = path + "/";;
}

