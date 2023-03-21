//#pragma once
//
//#include "Collider.h"
//
//namespace ECS {
//
//    class BoxCollider : public Collider {
//
//    public:
//
//        BoxCollider();
//
//        ~BoxCollider();
//
//        void init() override;
//
//        void start() override;
//
//        void update(float deltaTime) override;
//
//        float getVolume() override;
//
//        Vector2D getSize();
//        void setSize(float x, float y);
//
//        Vector2D getOffSet();
//        void addOffSet(float x, float y);
//
//        b2PolygonShape* getShape();
//
//    private:
//
//        Vector2D size;
//        Vector2D offSet;
//
//        // Box2d polygonShape
//        b2PolygonShape* shape;
//
//        // Just for comfort
//        b2Vec2* framePosition;
//
//        // Frame fixture settings
//        b2FixtureDef* frameFixtureDef;
//
//    };
//
//}
//
