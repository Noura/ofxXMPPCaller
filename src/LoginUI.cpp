//
//  LoginUI.cpp
//  UI component to handle reading in login information
//
//  TODO rewrite to inherit from a canvas and overwrite keyPressed
//  to switch text input
//
//  Created by Catherine Liu on 7/24/14.
//
//

#include "LoginUI.h"

LoginUI::LoginUI(float _x, float _y, float _w, float _h, ofxUICanvas * _sharedFonts)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, sharedFonts(_sharedFonts)
, visible(true) {
    setup();
}

LoginUI::LoginUI(float _x, float _y, float _w, float _h)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, sharedFonts(NULL)
, visible(true) {
    setup();
}

LoginUI::~LoginUI() {
    
    delete loginGUI;
    loginGUI=NULL;
}
void LoginUI::update(){
    loginGUI->update();
}
void LoginUI::draw(){
    if (visible)
        loginGUI->draw();
}

void LoginUI::switchInputs(){
    if(usernameInput->isClicked()){
        usernameInput->unClick();
        passwordIn->setFocus(true);
    }
    else{
        passwordIn->unClick();
        usernameInput->setFocus(true);
    }
    
}
void LoginUI::setVisible(bool _visible){
    visible = _visible;
    loginGUI->setVisible(_visible);
}
void LoginUI::setup(){
    if(sharedFonts)
        loginGUI = new ofxUICanvas(x,y,w,h, sharedFonts);
    else
        loginGUI = new ofxUICanvas(x,y,w,h);
    loginGUI->setWidgetSpacing(10);
    
    usernameLabel  = new ofxUILabel("usernameLabel", "Username: ", OFX_UI_FONT_LARGE);
    loginGUI->addWidgetDown(usernameLabel);
    
    passwordLabel  = new ofxUILabel("passwordLabel", "Password: ", OFX_UI_FONT_LARGE);
    loginGUI->addWidgetSouthOf(passwordLabel, "usernameLabel");
    
    passwordIn = new passwordInput("passwordInput", password, 240, 16, 0, 0, OFX_UI_FONT_MEDIUM);
    passwordIn->setDrawOutline(true);
    passwordIn->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);
    passwordIn->setAutoClear(false);
    loginGUI->addWidgetEastOf(passwordIn, "passwordLabel");
    
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
    
    ofAddListener(loginButton->mousePressed, this, &LoginUI::trigger);
    //TODO Figure out how to trigger login on enter and switch inputs on tab in this class
    
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

void LoginUI::trigger(bool &e){
    
        password = passwordIn->getTextString();
        user = usernameInput->getTextString();
        bool value = true;
        ofNotifyEvent(inputSubmitted, value, this);
}
void LoginUI::setBackgroundColor(ofColor color){
    loginGUI->setColorBack(color);
}
