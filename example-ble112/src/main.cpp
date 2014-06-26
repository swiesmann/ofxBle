#include "testApp.h"
#include "ofAppGlutWindow.h"



//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, WIN_WIDTH, WIN_HEIGTH, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
