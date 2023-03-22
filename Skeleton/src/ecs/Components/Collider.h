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

        enum BODY_TYPE {
            STATIC = 0, KINEMATIC = 1, DYNAMIC = 2
        };

        Collider();

        ~Collider();

        void init() override;

        void start() override;

        void update(float deltaTime);

        void fixedUpdate(float fixedDeltaTime);

        virtual float getVolume();

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

        b2Body* getBody();
        b2Fixture* getFixture();

        Vector2D getSize();
        void setSize(float x, float y);
        
        Vector2D getOffSet();
        void addOffSet(float x, float y);

        void setRotationFreezed(bool freezed);
        bool isRotationFreezed();
        
        void setBodyType(BODY_TYPE type);
        BODY_TYPE getBodyType();
        void setLinearDrag(float drag);
        float getLinearDrag();
    public:
        void setAngularDrag(float drag);
        float getAngularDrag();
                
        void setGravityScale(float scale);
        float getGravityScale();
        
        void setMass(float mass);
        float getMass();
        
        b2PolygonShape* getShape();

    private:

        Vector2D size;
        Vector2D offSet;

        Vector2D lastSync;

        float screenToWorldFactor;

        BODY_TYPE bodyType;

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
        float mass;

        // Box2d properties
        b2BodyDef* bodyDef;
        b2Body* body;
        b2FixtureDef* fixtureDef;
        b2Fixture* fixture;

        // Entity Components
        Transform* transform;

        // Box2d polygonShape
        b2PolygonShape* shape;
        
        // Just for comfort
        b2Vec2* framePosition;
        
    };
}

//#pragma once
//
//#include "Component.h"
//#include <box2d/b2_fixture.h>
//#include <Vector2D.h>
//
//class b2BodyDef;
//class b2Body;
//class b2FixtureDef;
//class b2World;
//enum b2BodyType;
//
//using namespace Utilities;
//
//namespace ECS {
//
//    class Transform;
//
//    class Collider : public Component {
//
//    public:
//
//        Collider();
//
//        ~Collider();
//
//        void init() override;
//
//        void update(float deltaTime) override = 0;
//
//        virtual float getVolume() = 0;
//
//        // Setters & getters
//        void setTrigger(bool trigger);
//        bool isTrigger();
//
//        void setFriction(float friction);
//        float getFriction();
//
//        void setBounciness(float bounciness);
//        float getBounciness();
//
//        b2Body* getBody();
//        b2Fixture* getFixture();
//
//    protected:
//
//        float screenToWorldFactor;
//
//        // Box2d
//        b2World* world;
//
//        // References to transforms properties
//        Vector2D* position;
//        const double* rotation;
//        Vector2D* scale;
//
//        // Fixture properties
//        bool trigger;
//        float friction;
//        float bounciness;
//
//        // Box2d properties
//        b2BodyDef* bodyDefinition;
//        b2Body* body;
//        b2Fixture* fixture;
//
//        // Entity Components
//        Transform* transform;
//    };
//
//}
