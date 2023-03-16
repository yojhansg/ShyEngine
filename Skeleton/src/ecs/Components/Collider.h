#pragma once

#include "Component.h"
#include "Vector2D.h"
#include <box2d/b2_fixture.h>

class b2BodyDef;
class b2Body;
class b2PolygonShape;
class b2FixtureDef;
class b2Fixture;
class b2World;
enum b2BodyType;

namespace ECS {

    class Transform;

    class Collider : public Component {

    public:

        Collider();

        ~Collider();

        void init() override;

        void start() override;

        void update(float deltaTime) override;

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

        void setRotationFreezed(bool freezed);
        bool getRotationFreezed();

        Utilities::Vector2D getSize();
        void setSize(float x, float y);

        Utilities::Vector2D getOffSet();
        void addOffSet(float x, float y);

        b2Body* getBody();
        b2PolygonShape* getShape();
        b2Fixture* getFixture();

    private:

        float screenToWorldFactor;

        Utilities::Vector2D size;
        Utilities::Vector2D offSet;

        // Fixture properties
        bool trigger;
        float friction;
        float bounciness;
        bool rotationFreezed;

        // Just for comfort
        b2Vec2* framePosition;

        // Position, Rotation & Scale
        Utilities::Vector2D* position;
        const double* rotation;
        Utilities::Vector2D* scale;

        // Box2d
        b2World* world;

        b2BodyDef* bodyDefinition;
        b2Body* body;
        b2PolygonShape* shape;
        b2Fixture* fixture;

        // Frame fixture settings
        b2FixtureDef* frameFixtureDef;

        // Entity Components
        Transform* transform;
    };

}
