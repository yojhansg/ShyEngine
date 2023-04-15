#pragma once
#include "Component.h"

#include <string>

namespace Sound {
	class SoundEffect;
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

		// Setters & getters
		void enableStartPlaying(bool enable);
		bool startsPlaying();

	private:

		reflect std::string fileName;
		reflect bool loop;
		reflect bool startPlaying;

		Transform* transform;

		Sound::SoundEffect* soundEffect;

		int channel;

	};


}