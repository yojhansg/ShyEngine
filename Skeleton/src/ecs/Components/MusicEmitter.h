#pragma once

#include "Component.h"

namespace Sound {
    class Music;
}

namespace ECS {

    class Transform;

    class MusicEmitter : public Component {

    public:

        MusicEmitter(cstring fileName);

        void init() override;

        void start() override;

    publish:

        void changeMusic(cstring musicPath);

        void play();

        void playWithFadeIn(int ms, int loops = -1);

        void fadeOut(int ms);

    private:

        Transform* transform;

        reflect std::string fileName;
        reflect bool loop;
        reflect bool startPlaying;

        Sound::Music* music;

    };
}

