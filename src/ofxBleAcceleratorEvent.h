#ifndef _OFXBLEACCELERATOREVENT_H
#define _OFXBLEACCELERATOREVENT_H


#include "ofMain.h"

class ofxBleAcceleratorEvent : public ofEventArgs {

public:

    uint16_t acc_x;
    uint16_t acc_y;

    ofxBleAcceleratorEvent() {
        acc_x = 0;
        acc_y = 0;
    }

    static ofEvent <ofxBleAcceleratorEvent> events;
};


#endif // _OFXBLEACCELERATOREVENT_H
