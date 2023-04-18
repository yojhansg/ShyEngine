#pragma once
#include "Component.h"

namespace Sound {
	class SoundEffect;
	class SoundManager;
}

namespace ECS {

	class Transform;

	class SoundEmitter : public Component {

	public:

		SoundEmitter(cstring fileName);

		void init() override;

		void start() override;

		void update(float deltaTime) override;

	publish:

		void changeSound(cstring soundPath);

		void play();

		void playWithfadeIn(int ms, int loops = -1);

		void fadeOut(int ms);

		// Setters & getters
		void enableStartPlaying(bool enable);
		bool startsPlaying();

		void setLoop(bool loop);
		bool isLoop();

		void setVolume(int volume);
		int getVolume();

	private:

		void resetChannel();

		reflect std::string fileName;
		reflect bool loop;
		reflect bool startPlaying;
		reflect int volume;

		Transform* transform;

		Sound::SoundEffect* soundEffect;

		Sound::SoundManager* soundManager;

		int channel;

		bool playing;

	};


}