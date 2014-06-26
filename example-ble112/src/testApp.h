#ifndef _TESTAPP_H
#define _TESTAPP_H



/**
 *
 * http://opengameart.org/content/pack-spaceships-and-building
 *
 */




#include "ofMain.h"
#include "ofColor.h"
#include "ofxBle.h"
#include "ship.h"
#include "Particle.h"
#include <time.h>

static const int WIN_WIDTH = 1024;
static const int WIN_HEIGTH = 768;

class testApp : public ofBaseApp {
	public:

	    Ship myShip;
	    vector<Ship> enemyShips;
	    vector<string> shipGfx;
	    vector<Particle> particles;

	    uint16_t centerX;
	    uint16_t centerY;
	    uint16_t lastAccX = -1;
	    uint16_t max_x = -1;
	    uint16_t min_x = 10000;
	    uint16_t const max_particles = 100;
	    uint16_t const max_enemies = 1;

	    clock_t last_ship_created_time;


		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();

//        void evtAcceleratorX(uint16_t &acc_x);
//        void evtAcceleratorY(uint16_t &acc_y);
        void evtAccelerator(ofxBleAcceleratorEvent &e);
        void createParticles( bool startupMode );
        void createEnemyShips();

        bool static checkParticleDead( Particle &p );
        bool static checkShipDead( Ship &p );


		// create object
        ofxBleThread bleThread;
};


#endif // _TESTAPP_H

