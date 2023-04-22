#pragma once

#include "Component.h"
#include <vector>

namespace Sound {
    class Music;
}

namespace ECS {

    class Transform;

    class MusicEmitter : public Component {

    public:

        MusicEmitter(cstring fileName);

        ~MusicEmitter();

        void init() override;

        void start() override;

    publish:

        void changeMusic(cstring musicPath);

        void play();

        void pause();

        void stop();

        void resume();

        void playWithFadeIn(float seconds, int loops = -1);

        void fadeOut(float seconds);

        bool isPlaying();

        bool isPaused();

        void rewind();

        void setVolume(float volume);

        float getVolume();

        void shouldPlayOnStart(bool play);

    private:

        static MusicEmitter* currentEmitterOnChannel;

        reflect std::string fileName;
        reflect bool loop;
        reflect bool playOnStart;
        reflect float volume;

        Transform* transform;

        Sound::Music* music;

        bool onChannel;

    };
}

