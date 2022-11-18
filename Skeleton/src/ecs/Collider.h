#pragma once

#include "Component.h"
#include "Vector2D.h"

namespace ECS {

    class Collider : public Component {

    public:

        Collider();

        void start() override;

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

        Utilities::Vector2D getSize();
        void setSize(Utilities::Vector2D size);

    private:

        float friction, bounciness;

        Utilities::Vector2D size;

        bool isTrigger_;
    };

}
