#pragma once
#include "Component.h"
#include <vector>

namespace Sound {
	class SoundEffect;
	class SoundManager;
}

namespace Scripting {
	class ScriptFunctionality;
}

namespace ECS {

	class Transform;

	class SoundEmitter : public Component {

	public:

		SoundEmitter();

		~SoundEmitter();

		void init() override;

		void start() override;

		void update(float deltaTime) override;

	publish:

		// Select another sound asset
		void changeSound(cstring soundPath);

		// Plays the current sound
		void play();

		// Pause the current sound. Can be resumed
		void pause();

		// Stop the current sound. Can not be resumed
		void stop();

		// If the sound is paused, resumes it
		void resume();

		// Plays the sound with fade in for (float seconds)
		// Loops-> Times the sound will repeat, -1 to "Infinitely", 0 to repeat just once
		void playWithfadeIn(int seconds, int loops = -1);

		// Fade out the sound for (float) seconds
		void fadeOut(int seconds);

		// Returns true if this component has the sound playing, false otherwise
		bool isPlaying();

		// Returns true if this component has the sound paused, false otherwise
		bool isPaused();

		// Sets the volume of the sound in this component
		// Value must be between 0 and 1
		void setVolume(float volume);
		
		// Gets the volume of the sound in this component
		// Returned value is betwween 0 and 1
		int getVolume();

		// Set true if you want to play the music at the 
		// start of the scene, false otherwise
		void shouldPlayOnStart(bool play);

		// Set true if you want to play the sound on loop, 
		// false otherwise
		void setLoop(bool loop);

		// Returns true if the sound is playing on loop,
		// false otherwise
		bool isOnLoop();

		// Set true to enable the spatial sound 
		// based on the camera position
		void enableSpatialSound(bool enable);

		// Returns true if spatial sound is enabled
		bool isSpatialSoundEnabled();

		// Sets the panning value, 0 to apply sound
		// only on the left channel, 1 to apply only on the right one
		void setPanning(float panning);

		// Return a value between 0 and 1 representing
		// how sound is played on left and right channels
		float getPanning();

		// Sets the distance at which sound ceases to be audible
		void setAudibleDistance(float distance);

		// Gets the distance at which sound ceases to be audible
		float getAudibleDistance();

	private:

		void disableEmittersOnSameChannel();

		void applyPanning();

		reflect std::string fileName;
		reflect bool loop;
		reflect bool playOnStart;
		reflect float volume;
		reflect bool spatialSound;
		reflect float panning;
		reflect float audibleDistance;

		static std::vector<SoundEmitter*> soundEmitters;

		Scripting::ScriptFunctionality* sFunctionality;

		Transform* transform;

		Sound::SoundEffect* soundEffect;

		Sound::SoundManager* sManager;

		float angle;

		int channel;

		int sound_id;

		bool onChannel;

	};


}