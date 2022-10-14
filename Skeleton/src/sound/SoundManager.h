#pragma once

#include "Singleton.h"
#include "SDL_mixer.h"

#include <string>
#include <map>

#define MAX_SOUND_DISTANCE 255

namespace SoundManager {

	class SoundManager : public Singleton<SoundManager> {

		friend Singleton<SoundManager>;

	public:

		enum SOUND_TYPE
		{
			SOUND_MUSIC = 0,
			SOUND_SFX = 1
		};

		~SoundManager();

		// Load and store by id a sound or a music depending on the type
		void load(const std::string& fileName, const std::string& id, SOUND_TYPE type);

		// Play a sound specifying a channel. -1 to find the first available channel.
		// Returns the selected channel or -1 if sound could not be played
		int playSound(const std::string& id, int loop, int channel = -1);
		
		// No need to specify a channel, music has only 1 channel
		void playMusic(const std::string& id, int loop);

		// Play a sound with fade in specifying a channel. -1 to find the first available channel
		void playWithFadeIn(int channel, const std::string& id, int loops, int ms);

		// Play the music with ms of fade in
		void playMusicWithFadeIn(const std::string& id, int loops, int ms);

		// Halt the music channel after fading out for ms
		void fadeOutMusic(int ms);

		void setMasterVolume(float volume);


		// Channels API

			// Pause a particular channel
			void pauseChannel(int channel);
			
			// Pause all channels
			void pauseAllChannels();

			// Return 1 if channel is paused, 0 otherwise
			bool pausedChannel(int channel);

			// Returns the number of paused channels
			int numberOfPausedChannels();

			// Resume a channel
			void resumeChannel(int channel);

			// Resume all channels
			void resumeAllChannels();

			// Returns non-zero if channel is playing, zero otherwise.
			bool isChannelPlaying(int channel);

			// Return the total number of channel playings
			int numberOfChannelsPlaying();

			// Set volume to a channel. Volume is between 0 and 128
			void setChannelVolume(int channel, int volume);

			// Get a channel volume. Return value is between 0 and 128
			int getChannelVolume(int channel);

			// Sets the position of the cannel relative to the listener. Angle is between 0-360 and distance is between 0-255
			void setChannelPosition(int channel, Sint16 angle, Uint8 distance);

		// Music API

			// Pause the music channel. It can be resumed
			void pauseMusic();

			// Returns 1 if music is paused, 0 otherwise
			bool musicPaused();

			// Stop the actual music object so music it's paused and it can not be resumed. User needs to create a new music object
			void haltMusic();

			// Resume the music channel
			void resumeMusic();

			// Returns 1 if music is playing, 0 otherwise
			bool musicPlaying();

			// Set the music channel volume.  Volume is between 0 and 128
			void setMusicVolume(int volume);

			// Gets the music channel volume. Return value is between 0 and 128
			int getMusicVolume();

			// Rewinds the music
			void rewingMusic();
		
	private:

		SoundManager();

		void initSDLMixer();
		void closeSDLMixer();

		std::map<std::string, Mix_Chunk*> sfxs;
		std::map<std::string, Mix_Music*> music;
	};
}