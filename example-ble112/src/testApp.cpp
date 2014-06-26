#include "testApp.h"
#include "ofxBleAcceleratorEvent.h"




//------------------------------------------------------------------------------
void testApp::setup(){

    ofSetWindowTitle("ofxBle ble112 example");
    ofBackground(0, 0, 0);
    centerX = WIN_WIDTH  / 2;
    centerY = WIN_HEIGTH / 2;

    // create player's ship
    myShip = Ship();
    myShip.setPos(new ofPoint(centerX, WIN_HEIGTH - 90));
    myShip.loadImage("medfighter.png");

    // load all ship
    shipGfx.push_back(string("bgbattleship.png"));
    shipGfx.push_back(string("bgspeedship.png"));
    shipGfx.push_back(string("heavyfreighter.png"));
    shipGfx.push_back(string("smallfreighterspr.png"));
    shipGfx.push_back(string("medfrighter.png"));
    shipGfx.push_back(string("speedship.png"));
    shipGfx.push_back(string("spshipspr1.png"));
    shipGfx.push_back(string("xspr5.png"));

    // don't spawn the first enemy ship right away
    last_ship_created_time = clock();

    // listen to the accelerator events
    ofAddListener(ofxBleAcceleratorEvent::events, this,
                  &testApp::evtAccelerator);

    // fill the window with starts
    createParticles(true);

    // start the thread
    printf("thread start");
    bleThread.startThread(false, true);    // blocking, verbose
}

//------------------------------------------------------------------------------
void testApp::evtAccelerator(ofxBleAcceleratorEvent &e) {

    //NOTE: use Y coord for X movement since the accelerometer is easier to hold
    //myShip.setPos(new ofPoint(e.acc_y, myShip.getY()));

    // set initial value
    if(lastAccX == -1)
    {
        lastAccX = e.acc_y;
        return;
    }



    if(e.acc_y > 405) {
        myShip.setMovement(new ofPoint(-1.2f, 0.0f));
    }
    else if (e.acc_y < 330){
        myShip.setMovement(new ofPoint(1.2f, 0.0f));
    }
    else {
        myShip.setMovement(new ofPoint(0.0f, 0.0f));
    }
}

//------------------------------------------------------------------------------
void testApp::update(){

    // remove particles that have left the screen
    ofRemove(particles, checkParticleDead);

    // remove ships that have left the screen
    ofRemove(enemyShips, checkShipDead);

    createParticles(false);
    createEnemyShips();

    // update all particles
    vector<Particle>::iterator particle_iter = particles.begin();
    while( particle_iter != particles.end() ){
        (*particle_iter).update();
        ++particle_iter;
    }

    // update all enemy ships
    vector<Ship>::iterator ship_iter = enemyShips.begin();
    while( ship_iter != enemyShips.end() ){

        (*ship_iter).update();
        ++ship_iter;
    }

    myShip.update();


    // check for ship collisions
    uint16_t myship_upper_y = (myShip.getY());
    uint16_t myship_left_x  = (myShip.getX());
    uint16_t myship_right_x = (myShip.getX() + (myShip.width));
    uint16_t current_ship_upper_y;
    uint16_t current_ship_right_x;
    uint16_t current_ship_left_x;
    Ship currentShip;
    ship_iter = enemyShips.begin();
    while( ship_iter != enemyShips.end() ){
        currentShip = (*ship_iter);

        current_ship_upper_y = ((*ship_iter).getY() + (*ship_iter).height);
        current_ship_right_x = (*ship_iter).getX() + (*ship_iter).width;
        current_ship_left_x  = (*ship_iter).getX();

        if( current_ship_upper_y > myship_upper_y ) {
            if(myship_left_x < current_ship_right_x && myship_left_x > current_ship_left_x ) {
                (*ship_iter).setMovement(new ofPoint(0.0f, 0.0f));
            }
            else if(myship_right_x < current_ship_right_x && myship_right_x > current_ship_left_x ) {
                (*ship_iter).setMovement(new ofPoint(0.0f, 0.0f));
            }
            else {
                (*ship_iter).setMovement(new ofPoint(0.0f, 0.5f));
            }
        }

        ++ship_iter;
    }
}

//------------------------------------------------------------------------------
void testApp::draw(){

    vector<Particle>::iterator it = particles.begin();
    while( it != particles.end() ){
        (*it).draw();
        ++it;
    }

    // fix the last color from drawing
    // particles so the images don't get colored
    ofSetColor(255, 255, 255);

    // draw all enemy ships

    vector<Ship>::iterator ship_iter = enemyShips.begin();
    while( ship_iter != enemyShips.end() ){
        (*ship_iter).draw();
        ++ship_iter;
    }

    // draw the player's ship
    myShip.draw();
}

//------------------------------------------------------------------------------
void testApp::keyPressed(int key){

}

//------------------------------------------------------------------------------
void testApp::keyReleased(int key){

}

//------------------------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//------------------------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//------------------------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//------------------------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//------------------------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//------------------------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//------------------------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//------------------------------------------------------------------------------
void testApp::exit() {

    // stop the thread
    bleThread.stopThread();
}

//------------------------------------------------------------------------------
bool testApp::checkParticleDead( Particle &p ){
    return p.getY() > WIN_HEIGTH;
}

//------------------------------------------------------------------------------
bool testApp::checkShipDead( Ship &sp ){
    return sp.getY() > WIN_HEIGTH;
}

//------------------------------------------------------------------------------
void testApp::createParticles( bool startupMode ) {

    float rnd;
    float start_y = -20;

    ofColor* rndColor;

    // create some particles
    while(particles.size() < max_particles) {
        rnd = ofRandom(0.0, 3.0);

        if(startupMode)
            start_y = ofRandomHeight();

        // just gray stars
        //color = (rnd * 70) + 45;
        //particles.push_back(Particle(ofColor(color, color, color), ofRandomWidth(), -20, 0.8, ofPoint(0, 4.0f-rnd)));

        // colorful stars
        rndColor = new ofColor();
        rndColor->r = (rnd * 70) + (ofRandom(0.0, 1.0) * 45);
        rndColor->g = (rnd * 70) + (ofRandom(0.0, 1.0) * 45);
        rndColor->b = (rnd * 70) + (ofRandom(0.0, 1.0) * 45);

        particles.push_back(
            Particle(
                rndColor,
                ofRandomWidth(),
                start_y,
                0.8,
                ofPoint(0, 4.0f-rnd)
            )
        );

    }
}

//------------------------------------------------------------------------------
void testApp::createEnemyShips( ) {


    float secs_past_last_ship = float(clock () - last_ship_created_time) /  CLOCKS_PER_SEC;
    if(secs_past_last_ship < ofRandom(0.03f, 1.3f)) {
        return;
    }


    //float rnd;
    //int color;
    float start_y = -100;
    Ship new_ship;


    // create some enemy ships
    if(enemyShips.size() < max_enemies) {
        //rnd = ofRandom(0.0, 1.0);

        new_ship = Ship();
        new_ship.setPos(new ofPoint(ofRandomWidth(), start_y));
        new_ship.loadImage(shipGfx.at( (uint16_t)ofRandom(0.0, shipGfx.size()-1 ) ));
        new_ship.setMovement(new ofPoint(0, ofRandom(1.0, 1.7)));


        enemyShips.push_back(new_ship);


        // just gray stars
        //color = (rnd * 70) + 45;
        //particles.push_back(Particle(ofColor(color, color, color), ofRandomWidth(), -20, 0.8, ofPoint(0, 4.0f-rnd)));



        //printf("rnd: %f\n", rnd);
        //printf("color(%i, %i, %i)\n", color, color, color);
    }

    last_ship_created_time = clock();

}
