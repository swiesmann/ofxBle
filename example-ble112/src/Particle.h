#ifndef PARTICLE_H
#define PARTICLE_H


#include "ofMain.h"


//-----------------------------------

class Particle {

    public:
        Particle(ofColor* color, float x, float y, float radius, ofPoint ptMovement);
        ~Particle();
        void draw();
        void update();
        float getY();
        float getX();


    private:
        ofColor* _color;
        ofPoint _ptMovement;
        float _x;
        float _y;
        float _radius;
};



#endif // PARTICLE_H
