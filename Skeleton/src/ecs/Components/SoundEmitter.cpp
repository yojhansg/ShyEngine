#include "SoundEmitter.h"
#include "SoundEffect.h"
#include "Transform.h"
#include "Entity.h"

#include <ScriptFunctionality.h>
#include <ResourcesManager.h>
#include <RendererManager.h>
#include <ConsoleManager.h>
#include <SoundManager.h>
#include <cmath>

#define MIX_MAX_PANNING 255
#define MIX_AUDIBLE_DISTANCE 255

namespace ECS {

	std::vector<SoundEmitter*> SoundEmitter::soundEmitters = std::vector<SoundEmitter*>();

	SoundEmitter::SoundEmitter() {

		sManager = Sound::SoundManager::instance();

		sFunctionality = Scripting::ScriptFunctionality::instance();

		transform = nullptr;
		soundEffect = nullptr;

		loop = false;
		playOnStart = true;
		onChannel = false;
		channel = -1;
		panning = .5f;
		spatialSound = false;
		audibleDistance = 0;

		volume = .5f;	

		this->fileName = "";

		soundEmitters.push_back(this);
	}

	SoundEmitter::~SoundEmitter() {
		stop();
	}

	void SoundEmitter::init() {

		transform = getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		loadSound(fileName);

		sound_id = soundEffect->getSoundId();

		float halfWidth = Renderer::RendererManager::instance()->getWidth() / 2;
		float haldHeight = Renderer::RendererManager::instance()->getHeight() / 2;

		audibleDistance = Utilities::Vector2D(halfWidth, haldHeight).magnitude();

	}

	void SoundEmitter::start() {

		if (playOnStart)
			play();

	}

	void SoundEmitter::update(float deltaTime) {

		if (onChannel && spatialSound) {

			auto distanceToCamera = (transform->GetLocalPosition() - sFunctionality->Camera_GetPosition());

			auto dNormalized = distanceToCamera.normalize();

			int16_t angle = atan2(dNormalized.getX(), dNormalized.getY()) * 180 / M_PI;

			int d = distanceToCamera.magnitude() * MIX_AUDIBLE_DISTANCE / audibleDistance;

			if (d > MIX_AUDIBLE_DISTANCE) d = MIX_AUDIBLE_DISTANCE;

			sManager->setChannelPosition(channel, angle, d);

		}

	}

	void SoundEmitter::loadSound(cstring soundPath) {
		fileName = soundPath;

		soundEffect = Resources::ResourcesManager::instance()->addSound(fileName);
	}

	void SoundEmitter::play() {

		onChannel = true;

		channel = sManager->playSound(sound_id, loop);

		sManager->setChannelVolume(channel, volume * MIX_MAX_VOLUME);

		applyPanning();

		disableEmittersOnSameChannel();

	}

	void SoundEmitter::playWithfadeIn(int seconds, int loops) {

		onChannel = true;

		sManager->setChannelVolume(channel, volume * MIX_MAX_VOLUME);

		channel = sManager->fadeInChannel(channel, sound_id, loops, seconds * 1000.0f);

		applyPanning();

		disableEmittersOnSameChannel();

	}

	void SoundEmitter::pause() {
		if (onChannel)
			sManager->pauseChannel(channel);
	}

	void SoundEmitter::stop() {
		if (onChannel)
			sManager->stopChannel(channel);
	}

	void SoundEmitter::resume() {
		if (onChannel)
			sManager->resumeChannel(channel);
	}

	void SoundEmitter::fadeOut(int seconds) {

		if (onChannel)
			sManager->fadeOutChannel(channel, seconds * 1000.0f);

	}

	bool SoundEmitter::isPlaying() {
		if (onChannel)
			return sManager->isChannelPlaying(channel);

		return false;
	}

	bool SoundEmitter::isPaused() {
		if (onChannel)
			return sManager->pausedChannel(channel);

		return false;
	}

	void SoundEmitter::setVolume(float volume) {

		if (volume < 0.0f || volume > 1.0f) {
			Console::Output::PrintError("Invalid volume argument", "Volume value must be between 0 and 1!");
			return;
		}

		this->volume = volume;
	}

	int SoundEmitter::getVolume() {
		return volume;
	}

	void SoundEmitter::shouldPlayOnStart(bool play) {
		this->playOnStart = play;
	}

	void SoundEmitter::setLoop(bool loop) {
		this->loop = loop;
	}

	bool SoundEmitter::isOnLoop() {
		return loop;
	}

	void SoundEmitter::enableSpatialSound(bool enable) {
		this->spatialSound = enable;
	}

	bool SoundEmitter::isSpatialSoundEnabled() {
		return spatialSound;
	}

	void SoundEmitter::setPanning(float panning) {

		if (volume < 0.0f || volume > 1.0f) {
			Console::Output::PrintError("Invalid volume argument", "Panning value must be between 0 and 1!");
			return;
		}

		this->panning = panning;

		if (onChannel)
			applyPanning();
	}

	float SoundEmitter::getPanning() {
		return panning;
	}

	void SoundEmitter::setAudibleDistance(float distance) {
		this->audibleDistance = distance;
	}

	float SoundEmitter::getAudibleDistance() {
		return audibleDistance;
	}

	void SoundEmitter::disableEmittersOnSameChannel() {
		for (auto s : soundEmitters)
			if (s != nullptr && s != this)
				if (s->onChannel && s->channel == channel)
					s->onChannel = false;
	}

	void SoundEmitter::applyPanning() {
		if (!spatialSound)
			sManager->setChannelPanning(channel, MIX_MAX_PANNING - (panning * MIX_MAX_PANNING), panning * MIX_MAX_PANNING);
	}

}
