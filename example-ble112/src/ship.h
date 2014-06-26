#ifndef SHIP_H
#define SHIP_H

#include "ofImage.h"


class Ship : public ofImage
{
    public:
        Ship();
        Ship(ofPoint* ptPos);
        void draw();
        void update();
        float getY();
        float getX();
        void setPos(ofPoint* ptPos);
        void setMovement(ofPoint* ptMovement);
        virtual ~Ship();
    protected:
    private:
        ofPoint* _position;
        ofPoint* _movement;
};

#endif // SHIP_H
