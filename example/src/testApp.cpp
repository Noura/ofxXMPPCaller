#include "testApp.h"
#include <assert.h>

void testApp::setup(){
    /* code when xml contains email/password
    perhaps run try/catch for loading stored settings?
    
    ofXml settings;
	bool test = settings.load("settings.xml");
    assert(test);
    
	string server = settings.getValue("server");
	string user = settings.getValue("user");
	string pwd = settings.getValue("pwd");
    
    xmppCaller = new ofxXMPPCaller(0, 0, server, user, pwd, "Login", "telekinect");
    */
    xmppCaller = new ofxXMPPCaller(0,0,"Login", "telekinect");
    xmppCaller->setup();
}

void testApp::exit(){
    if (xmppCaller) delete xmppCaller;
}

void testApp::update(){
    xmppCaller->update();
}

void testApp::draw(){
    xmppCaller->draw();
}

/* Required method stubs for ofBaseApp ****************************************/

void testApp::keyPressed(int key) {}

void testApp::keyReleased(int key) {}

void testApp::mouseMoved(int x, int y ) {}

void testApp::mouseDragged(int x, int y, int button) {}

void testApp::mouseReleased(int x, int y, int button) {}

void testApp::mousePressed(int x, int y, int button) {}

void testApp::windowResized(int w, int h) {}

void testApp::dragEvent(ofDragInfo dragInfo) {}

void testApp::gotMessage(ofMessage msg) {}
