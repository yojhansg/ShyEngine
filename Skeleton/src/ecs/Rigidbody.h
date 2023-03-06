#pragma once

#include "Component.h"
#include <box2d/b2_fixture.h>

class b2BodyDef;
class b2Body;
class b2PolygonShape;
class b2FixtureDef;
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

        void freezeXPosition(bool value);

        void freezeYPosition(bool value);

        void freezeZRotation(bool value);

    private:

        b2BodyType assingBodyType(BODY_TYPE type);

        BODY_TYPE bodyType;

        float mass;
        float linearDrag;
        float angularDrag;

        float gravityScale;

        // Constraints
        bool xMovementLocked;
        bool yMovementLocked;
        bool rotationLocked;

        // Box 2D
        b2World* world;
        
        b2Body* body;
        b2BodyDef* bodyDefinition;
        b2FixtureDef* fixture;

        // Entity Components
        Transform* transform;
        Collider* collider;
    };
}