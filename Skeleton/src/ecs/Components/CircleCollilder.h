//#pragma once
//#include "Collider.h"
//
//namespace ECS {
//
//    class CircleCollilder : public Collider {
//
//    public:
//
//        CircleCollilder();
//
//        ~CircleCollilder();
//
//        void init() override;
//
//        void start() override;
//
//        void update(float deltaTime) override;
//
//        float getVolume() override;
//
//        void setRadiusToShorterAxis(bool value);
//
//        void setRadius(float radius);
//        float getRadius();
//
//        Vector2D getOffSet();
//        void addOffSet(float x, float y);
//
//        b2CircleShape* getShape();
//
//    private:
//
//        float calculateRadius();
//
//        Vector2D size;
//        Vector2D offSet;
//        
//        float radius;
//
//        bool shorterAxis;
//
//        // Box2d circleShape
//        b2CircleShape* shape;
//
//        // Just for comfort
//        b2Vec2* framePosition;
//
//    };
//
//}
//
