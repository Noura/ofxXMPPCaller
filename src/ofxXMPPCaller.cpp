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
, loginGUI(NULL)
, unlaunchCanvas(NULL)
, unlaunchButton(NULL),
 onLoginGui(false){
    appState.setCallCapability(_capability);
    xmpp.setShow(ofxXMPPShowAvailable);
     this->server = server;
     this->user = user;
     this->password = password;
    xmpp.setCapabilities(appState.callCapability);
    
    sharedFonts = new ofxUICanvas();
    sharedFonts->setFont("GUI/NewMediaFett.ttf");
    sharedFonts->setVisible(false);
}

ofxXMPPCaller::ofxXMPPCaller(float _x, float _y, string _launchButtonLabel, string _capability)
: x(_x)
, y(_y)
, launchButtonLabel(_launchButtonLabel)
, gui(NULL)
, loginGUI(NULL)
, unlaunchCanvas(NULL)
, unlaunchButton(NULL)
, onLoginGui(true){
    
    appState.setCallCapability(_capability);
    xmpp.setShow(ofxXMPPShowAvailable);
    this->server = "talk.google.com";
    this->user = "";
    this->password = "";
    xmpp.setCapabilities(appState.callCapability);
    
    sharedFonts = new ofxUICanvas();
    sharedFonts->setFont("GUI/NewMediaFett.ttf");
    sharedFonts->setVisible(false);
}

void ofxXMPPCaller::setup() {
    if (onLoginGui)
        unlaunch(onLoginGui);
    else
        launch(onLoginGui);
}

ofxXMPPCaller::~ofxXMPPCaller() {
    deletes();
    delete sharedFonts;
}

void ofxXMPPCaller::update() {
    if (gui) gui->update();
    if (loginGUI) loginGUI->update();
    if (unlaunchCanvas) unlaunchCanvas->update();
}

void ofxXMPPCaller::draw() {
    if (gui) gui->draw();
    if (loginGUI) loginGUI->draw();
    if (unlaunchCanvas) unlaunchCanvas->draw();
}

/// Sets the UI to the "closed" state, ready to be launched.
/// The dummy argument is so that this function can be triggered as an event callback.
void ofxXMPPCaller::unlaunch(bool & e) {
    deletes();
    // If there's an xmpp connection, close it but without crashing?
    //xmpp.stop();
    if(xmpp.getConnectionState()==ofxXMPPConnected){
        xmpp.stop();
        //delete xmpp;
        //TODO figure out how to stop being connected?
    }
    loginGUI = new LoginUI(x, y, 1024, 768, &appState, sharedFonts);
    loginGUI->setup();
    
    ofAddListener(loginGUI->inputSubmitted, this, &ofxXMPPCaller::launch);
}

/// Sets the UI to the "open" state, showing contacts to chat with. Also starts the xmpp connection
/// The dummy argument is so that this function can be triggered as an event callback.
void ofxXMPPCaller::launch(bool & e) {
    if(loginGUI==NULL || proccessLoginInfo()){
        deletes();
        if(xmpp.getConnectionState()!=ofxXMPPConnected)
            xmpp.connect(server, user, password);
        // gui is the chat UI
        gui = new CallingGUI(x, y, &appState, &xmpp, sharedFonts);
        gui->setup();
        
        float unlaunchW = 75;
        float unlaunchH = 40;
        float margin = 3;
        float unlaunchX = ofGetWidth() - unlaunchW;
        float unlaunchY = y;
        
        // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
        unlaunchCanvas = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH, sharedFonts);
        unlaunchButton = new CustomEventLabelButton("Logout", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
        unlaunchCanvas->addWidget(unlaunchButton);
        
        ofAddListener(unlaunchButton->mousePressed, this, &ofxXMPPCaller::unlaunch);
        
        ofColor dark(100, 100, 100);
        unlaunchCanvas->setColorBack(dark);
    }
}

void ofxXMPPCaller::deletes() {
    if (loginGUI) {
        //ofRemoveListener(loginButton->mousePressed, this, &ofxXMPPCaller::launch);
        delete loginGUI;
        loginGUI = NULL;
    }
    if (unlaunchCanvas) {
        ofRemoveListener(unlaunchButton->mousePressed, this, &ofxXMPPCaller::unlaunch);
        delete unlaunchCanvas;
        unlaunchCanvas = NULL;
        unlaunchButton = NULL;
    }
    if (gui) {
        
        //close xmpp connection without crashing?
        //xmpp.stop();
        delete gui;
        gui = NULL;
    }
}

bool ofxXMPPCaller::proccessLoginInfo(){
    string u = loginGUI->getUsername();
    string p = loginGUI->getPassword();
    
    if (u.length()<=0) {
        if (p.length()<=0){
            
            loginGUI->setWarning("Enter a username and password!");
            
        }
        else
            loginGUI->setWarning("Enter a username!");
        return false;
    }
    else if (p.length()<=0){
        
        loginGUI->setWarning("Enter a password!");
        return false;
        
    }
    else{
        user = u+"@gmail.com";
        password = p;
        //cout<<user+" "+password+"\n";
        
        /*clear fields
        usernameInput->setTextString("");
        passwordInput->setTextString("");
        */
        
        
        /*xml settings
        settings->setValue("settings:user", user+"@gmail.com");
        settings->setValue("settings:pwd", password);
        settings->saveFile("settings.xml");
         */
        return true;
    }
    
}