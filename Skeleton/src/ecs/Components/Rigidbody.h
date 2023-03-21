//#pragma once
//
//#include "Component.h"
//#include "Vector2D.h"
//#include <box2d/b2_fixture.h>
//
//class b2Body;
//class b2BodyDef;
//class b2Fixture;
//class b2PolygonShape;
//class b2World;
//enum b2BodyType;
//
//namespace ECS {
//
//    class Transform;
//
//    class Rigidbody : public Component {
//
//    public:
//
//        enum BODY_TYPE {
//            STATIC = 0, KINEMATIC = 1, DYNAMIC = 2
//        };
//
//        Rigidbody();
//
//        ~Rigidbody();
//
//        void init() override;
//
//        void fixedUpdate(float fixedDeltaTime) override;
//
//        void start() override;
//
//        void setRotationFreezed(bool freezed);
//        bool isRotationFreezed();
//
//        void setBodyType(BODY_TYPE type);
//        BODY_TYPE getBodyType();
//
//        void setLinearDrag(float drag);
//        float getLinearDrag();
//
//        void setAngularDrag(float drag);
//        float getAngularDrag();
//        
//        void setGravityScale(float scale);
//        float getGravityScale();
//
//        void setMass(float mass);
//
//
//
//
//        void setLinearVelocity(const Utilities::Vector2D& vel);
//
//        Utilities::Vector2D getLinearVelocity();
//
//        void applyForce(const Utilities::Vector2D& force);
//
//    private:
//
//        float screenToWorldFactor;
//
//        BODY_TYPE bodyType;
//
//        // Box 2D
//        b2World* world;
//        
//        b2BodyDef* bodyDef;
//        b2Body* body;
//        b2Fixture* fixture;
//        b2FixtureDef* fixtureDef;
//        b2PolygonShape* shape;
//
//        // Entity Components
//        Transform* transform;
//    };
//}