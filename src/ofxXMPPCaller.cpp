/** ofxXMPPCaller.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: noura
 */

#include "ofxXMPPCaller.h"

ofxXMPPCaller::ofxXMPPCaller(float _x, float _y, string server, string user, string password, string _launchButtonLabel, string _capability)
: x(_x)
, y(_y)
, launchButtonLabel(_launchButtonLabel)
, gui(NULL)
, launchCanvas(NULL)
, launchButton(NULL)
, unlaunchCanvas(NULL)
, unlaunchButton(NULL) {
    appState.setCallCapability(_capability);
    xmpp.setShow(ofxXMPPShowAvailable);
    xmpp.connect(server, user, password);
    xmpp.setCapabilities(appState.callCapability);
}

void ofxXMPPCaller::setup() {
    bool e = true;
    unlaunch(e);
}

ofxXMPPCaller::~ofxXMPPCaller() {
    deletes();
}

void ofxXMPPCaller::update() {
    if (gui) gui->update();
    if (launchCanvas) launchCanvas->update();
    if (unlaunchCanvas) unlaunchCanvas->update();
}

void ofxXMPPCaller::draw() {
    if (gui) gui->draw();
    if (launchCanvas) launchCanvas->draw();
    if (unlaunchCanvas) unlaunchCanvas->draw();
}

/// Sets the UI to the "closed" state, ready to be launched.
/// The dummy argument is so that this function can be triggered as an event callback.
void ofxXMPPCaller::unlaunch(bool & e) {
    deletes();
    float launchW = 75;
    float launchH = 40;
    float margin = 3;
    // launchCanvas and launchButton will launch or "open" the chat UI
    launchCanvas = new ofxUICanvas(x, y, launchW, launchH);
    launchButton = new CustomEventLabelButton(launchButtonLabel, launchW - 2.0 * margin, launchH - 2.0 * margin, x, y, OFX_UI_FONT_SMALL_SIZE);
    launchCanvas->addWidget(launchButton);
    
    ofAddListener(launchButton->mousePressed, this, &ofxXMPPCaller::launch);
    
    ofColor dark(100, 100, 100);
    launchCanvas->setColorBack(dark);
}

/// Sets the UI to the "open" state, showing contacts to chat with.
/// The dummy argument is so that this function can be triggered as an event callback.
void ofxXMPPCaller::launch(bool & e) {
    deletes();
    
    // gui is the chat UI
    gui = new CallingGUI(x, y, &appState, &xmpp);
    gui->setup();
    
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = ofGetWidth() - unlaunchW;
    float unlaunchY = y;
    
    // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
    unlaunchCanvas = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH);
    unlaunchButton = new CustomEventLabelButton("Close", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL_SIZE);
    unlaunchCanvas->addWidget(unlaunchButton);
    
    ofAddListener(unlaunchButton->mousePressed, this, &ofxXMPPCaller::unlaunch);
    
    ofColor dark(100, 100, 100);
    unlaunchCanvas->setColorBack(dark);
}

void ofxXMPPCaller::deletes() {
    if (launchCanvas) {
        ofRemoveListener(launchButton->mousePressed, this, &ofxXMPPCaller::launch);
        delete launchCanvas;
        launchCanvas = NULL;
        launchButton = NULL;
    }
    if (unlaunchCanvas) {
        ofRemoveListener(unlaunchButton->mousePressed, this, &ofxXMPPCaller::unlaunch);
        delete unlaunchCanvas;
        unlaunchCanvas = NULL;
        unlaunchButton = NULL;
    }
    if (gui) {
        delete gui;
        gui = NULL;
    }
}