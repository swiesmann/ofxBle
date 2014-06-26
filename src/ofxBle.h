#ifndef _OFXBLE_H
#define _OFXBLE_H

#include "ofMain.h"
#include "apitypes.h"
#include "ofxBleAcceleratorEvent.h"

#define OFXBLE_LINUX_BLE112


// source: http://stackoverflow.com/a/5920028/3061383
#ifdef _WIN64
    //define something for Windows (64-bit)
    #error "_WIN64 not implemented"

#elif _WIN32
    //define something for Windows (32-bit)
    #error "_WIN32 not implemented"

#elif __APPLE__
    #error "__APPLE__ not implemented"
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    #elif TARGET_OS_IPHONE
    // iOS device
    #elif TARGET_OS_MAC
    // Other kinds of Mac OS
    #else
    // Unsupported platform
    #endif

#elif __linux__

    #ifdef OFXBLE_LINUX_BLE112
        #include "ofxBle_linux_ble112.h"
    #elif defined(OFXBLE_LINUX_BLUEZ)
        #error "ofxBle on linux bluez not implemented"
    #endif

#elif __unix // all unices not caught above
    #error "__unix not implemented"

#elif __posix
    #error "__posix not implemented"

#else
    #error "unknown operating system"

#endif


#endif // _OFXBLE_H
