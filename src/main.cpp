#include "ofMain.h"
#include "ofApp.h"

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW); // setup the GL context
    ofSetFrameRate(12); // An appropriate framerate that moves the snake at a good speed
    // Due to the simple nature of our objects rendering things this slow should not cause visual discomfort or lage
    
    // this kicks off the running of my app
    ofRunApp(new snakelinkedlist::snakeGame());
}
