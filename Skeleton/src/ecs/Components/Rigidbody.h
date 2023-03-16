#pragma once

#include "Component.h"
#include "Vector2D.h"
#include <box2d/b2_fixture.h>

class b2Body;
class b2Fixture;
class b2World;
enum b2BodyType;

namespace ECS {

    class Transform;
    class Collider;

    class Rigidbody : public Component {

    public:

        enum BODY_TYPE {
            STATIC = 0, KINEMATIC = 1, DINAMIC = 2
        };

        Rigidbody();

        ~Rigidbody();

        void init() override;

        void fixedUpdate(float fixedDeltaTime) override;

        void setBodyType(BODY_TYPE type);

        void setLinearDrag(float drag);

        void setAngularDrag(float drag);

        void setMass(float mass);

        void setGravityScale(float scale);

        void setLinearVelocity(const Utilities::Vector2D& vel);

        Utilities::Vector2D getLinearVelocity();

        void applyForce(const Utilities::Vector2D& force);

    private:

        float screenToWorldFactor;

        float mass;
        float linearDrag;
        float angularDrag;
        float gravityScale;

        BODY_TYPE bodyType;

        // Box 2D
        b2World* world;
        
        b2Body* body;
        b2Fixture* fixture;

        // Entity Components
        Transform* transform;
        Collider* collider;
    };
}