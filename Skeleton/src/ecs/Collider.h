#pragma once

#include "Component.h"
#include "Vector2D.h"
#include <box2d/b2_fixture.h>

class b2BodyDef;
class b2Body;
class b2PolygonShape;
class b2FixtureDef;
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

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

        Utilities::Vector2D getSize();
        void setSize(Utilities::Vector2D size);

        b2Body* getBody();
        b2PolygonShape* getShape();

    private:

        // Collider properties
        float friction, bounciness;

        bool isTrigger_;

        Utilities::Vector2D size;

        // Box2d
        b2World* world;

        b2BodyDef* bodyDefinition;
        b2Body* body;
        b2PolygonShape* shape;

        // Entity Components
        Transform* transform;
    };

}
