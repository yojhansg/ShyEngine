#pragma once

#include "Component.h"
#include <box2d/b2_fixture.h>
#include <Vector2D.h>

class b2BodyDef;
class b2Body;
class b2FixtureDef;
class b2World;
class b2Shape;
enum b2BodyType;

using namespace Utilities;

namespace ECS {

    class Transform;

    class PhysicsBody : public Component {

    public:

        enum BODY_TYPE {
            STATIC = 0, KINEMATIC = 1, DYNAMIC = 2
        };

        PhysicsBody();

        ~PhysicsBody();

        void init() override;

        void start() override;

        void fixedUpdate(float fixedDeltaTime);

        // Setters & getters
    publish:
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();
    public:
        Vector2D getOffSet();
        void addOffSet(float x, float y);

        void setRotationFreezed(bool freezed);
        bool isRotationFreezed();
        
        void setBodyType(BODY_TYPE type);
        BODY_TYPE getBodyType();
        
        void setLinearDrag(float drag);
        float getLinearDrag();
        
        void setAngularDrag(float drag);
        float getAngularDrag();
                
        void setGravityScale(float scale);
        float getGravityScale();

        virtual void setMass(float mass) = 0;
        float getMass();
        
        virtual float getArea() = 0;

    protected:

        virtual void scaleShape() = 0;

        Vector2D size;
        Vector2D offSet;

        float screenToWorldFactor;

        BODY_TYPE bodyType;

        // Box2d
        b2World* world;

        // References to transforms properties
        Vector2D* position;
        const double* rotation;
        Vector2D* scale;

        // Fixture properties
        float mass;

        // Box2d properties
        b2BodyDef* bodyDef;
        b2Body* body;
        b2FixtureDef* fixtureDef;
        b2Fixture* fixture;

        // Entity Components
        Transform* transform;

    private:

        Vector2D lastPositionSync;
        float lastRotationSync;
        Vector2D lastScaleInfo;
    };
}
