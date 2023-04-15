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

    publish:

        void changeMusic(cstring musicPath);

    private:

        Transform* transform;

        reflect std::string fileName;

        Sound::Music* music;

    };
}

