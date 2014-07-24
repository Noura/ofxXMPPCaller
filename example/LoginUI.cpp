//
//  LoginUI.cpp
//  example
//
//  Created by Catherine Liu on 7/24/14.
//
//

#include "LoginUI.h"

LoginUI::LoginUI(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, ofxUICanvas * _sharedFonts)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, appState(_appState)
, sharedFonts(_sharedFonts){
    
}

LoginUI::~LoginUI() {
    delete loginGUI;
}
void LoginUI::update(){
    loginGUI->update();
}
void LoginUI::draw(){
    loginGUI->draw();
}

void LoginUI::setup(){
    // launchCanvas and launchButton will launch or "open" the chat UI
    loginGUI = new ofxUICanvas(x, y, w, h, sharedFonts);
    
    loginGUI->setWidgetSpacing(10);
    
    usernameLabel  = new ofxUILabel("usernameLabel", "Username: ", OFX_UI_FONT_LARGE);
    loginGUI->addWidgetDown(usernameLabel);
    
    passwordLabel  = new ofxUILabel("passwordLabel", "Password: ", OFX_UI_FONT_LARGE);
    loginGUI->addWidgetSouthOf(passwordLabel, "usernameLabel");
    
    passwordInput = new ofxUITextInput("passwordInput", password, 240, 16, 0, 0, OFX_UI_FONT_MEDIUM);
    passwordInput->setDrawOutline(true);
    passwordInput->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);
    passwordInput->setAutoClear(false);
    loginGUI->addWidgetEastOf(passwordInput, "passwordLabel");
    
    if(user.find("@gmail.com")!=string::npos)
        user.erase(user.length()-10, user.length());
    usernameInput = new ofxUITextInput("usernameInput", user, 240, 16, 0, 0, OFX_UI_FONT_MEDIUM);
    loginGUI->addWidgetNorthOf(usernameInput, "passwordInput");
    usernameInput->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);
    usernameInput->setDrawOutline(true);
    usernameInput->setAutoClear(false);
    
    emailLabel = new ofxUILabel("emailLabel", "@gmail.com", OFX_UI_FONT_MEDIUM);
    loginGUI->addWidgetEastOf(emailLabel, "usernameInput");
    
    
    loginButton = new CustomEventLabelButton("Login", 120, 16, x, y, OFX_UI_FONT_SMALL);
    loginButton->setDrawOutline(true);
    loginGUI->addWidgetSouthOf(loginButton, "passwordInput");
    
    warning = new ofxUILabel("warning", "", OFX_UI_FONT_MEDIUM);
    loginGUI->addWidgetSouthOf(warning, "Login");
    loginGUI->getWidget("warning")->setColorFill(ofxUIColor(255,0,0));
    
    loginGUI->centerWidgets();
    
    //TODO FIGURE OUT INPUTTRIGGERS
    ofAddListener(loginButton->mousePressed, this, &LoginUI::trigger);
    //ofAddListener(loginGUI->newGUIEvent, this, &LoginUI::trigger);
    ofColor dark(100, 100, 100);
    loginGUI->setColorBack(dark);
    
    loginGUI->setColorOutline(ofColor(255,255,255));
}

void LoginUI::setWarning(string warningMsg){
    warning->setLabel(warningMsg);
}
string LoginUI::getPassword(){
    return password;
}

string LoginUI::getUsername(){
    return user;
}
/*
void LoginUI::guiEvent(ofxUIEventArgs &e)
{
    bool ev = true;
    trigger(ev);
}*/

void LoginUI::trigger(bool &b){
    
    cout<<"triggered\n";
    password = passwordInput->getTextString();
    user = usernameInput->getTextString();
    bool value = true;
    ofNotifyEvent(inputSubmitted, value, this);
}


