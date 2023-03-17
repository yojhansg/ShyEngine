#pragma once

#include "Component.h"
#include <box2d/b2_fixture.h>
#include <Vector2D.h>

class b2BodyDef;
class b2Body;
class b2FixtureDef;
class b2World;
enum b2BodyType;

using namespace Utilities;

namespace ECS {

    class Transform;

    class Collider : public Component {

    public:

        Collider();

        ~Collider();

        void init() override;

        void update(float deltaTime) override = 0;

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

        void setRotationFreezed(bool freezed);
        bool isRotationFreezed();

        b2Body* getBody();
        b2Fixture* getFixture();

    protected:

        float screenToWorldFactor;

        // Box2d
        b2World* world;

        // References to transforms properties
        Vector2D* position;
        const double* rotation;
        Vector2D* scale;

        // Fixture properties
        bool trigger;
        float friction;
        float bounciness;
        bool rotationFreezed;

        // Box2d properties
        b2BodyDef* bodyDefinition;
        b2Body* body;
        b2Fixture* fixture;

        // Entity Components
        Transform* transform;
    };

}
