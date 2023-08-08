#pragma once

#include "Component.h"
#include <vector>

namespace Sound {
    class Music;
    class SoundManager;
}

namespace ECS {

    class Transform;

    class MusicEmitter : public Component {

    public:

        MusicEmitter();

        ~MusicEmitter();

        void init() override;

        void start() override;

    publish:

        // Select another music asset
        void changeMusic(cstring musicPath);

        // Plays the current music
        void play();

        // Pause the current music. Can be resumed
        void pause();

        // Stop the current music. Can not be resumed
        void stop();

        // If music is paused, resumes the music
        void resume();

        // Plays the music with fade in for (float seconds)
        // Loops-> Times music will repeat, -1 to "Infinitely", 0 to repeat just once
        void playWithFadeIn(float seconds, int loops = -1);

        // Fade out the music for (float) seconds
        void fadeOut(float seconds);

        // Returns true if this component has the music playing, false otherwise
        bool isPlaying();

        // Returns true if this component has the music paused, false otherwise
        bool isPaused();

        // Rewind the music from the beginning
        void rewind();

        // Sets the volume of the music in this component
        // Value must be between 0 and 1
        void setVolume(float volume);

        // Gets the volume of the music in this component
        // Returned value is betwween 0 and 1
        float getVolume();

        // Set true if you want to play the music at the 
        // start of the scene, false otherwise
        void shouldPlayOnStart(bool play);

        // Set true if you want to play the music on loop, 
        // false otherwise
        void setLoop(bool loop);

        // Returns true if the music is playing on loop,
        // false otherwise
        bool isOnLoop();

    private:

        reflect std::string fileName;
        reflect bool loop;
        reflect bool playOnStart;
        reflect float volume;

        Sound::SoundManager* sManager;

        Transform* transform;

        Sound::Music* music;

        static MusicEmitter* emitterOnChannel;

        bool onChannel;

        int music_id;

    };
}

