#include "Particle.h"


//------------------------------------------------------------------------------
Particle::Particle(ofColor* color, float x, float y, float radius,
                   ofPoint ptMovement)
{
    _color = color;
    _x = x;
    _y = y;
    _radius = radius;
    _ptMovement = ptMovement;
}

//------------------------------------------------------------------------------
Particle::~Particle()
{
    //dtor
}

//------------------------------------------------------------------------------
void Particle::draw()
{
    ofSetColor(*_color);
    ofCircle(_x, _y, _radius);
    //ofSetColor(255,255,255);
}

//------------------------------------------------------------------------------
void Particle::update()
{
    _x = _x + _ptMovement.x;
    _y = _y + _ptMovement.y;
}

//------------------------------------------------------------------------------
float Particle::getY() {
    return _y;
}

//------------------------------------------------------------------------------
float Particle::getX() {
    return _x;
}
