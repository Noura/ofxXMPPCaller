//
//  LoginUI.h
//  example
//
//  Created by Catherine Liu on 7/24/14.
//
//

#ifndef LOGINUI
#define LOGINUI
#endif
#include "ofxUI.h"
#include "SharedStateBundle.h"
#include "CustomEventLabelButton.h"
#include "ofMain.h"
#include "passwordInput.h"

class LoginUI {
    
public:
    
    LoginUI(float _x, float _y,  float _w, float _h, ofxUICanvas * _sharedFonts);
    LoginUI(float _x, float _y,  float _w, float _h);
    ~LoginUI();
    
    void setup();
    void update();
    void draw();
    
    string getUsername();
    string getPassword();
    void setWarning(string warningMsg);
    ofEvent<bool> inputSubmitted;
    void setBackgroundColor(ofColor color);
protected:
    
    float x, y, w, h;
    
    
    //void trigger(ofxUIEventArgs &e);
    void trigger(bool &e);
    
    ofxUICanvas * loginGUI;
    ofxUICanvas * sharedFonts;
    string user, password;
    ofxUITextInput *usernameInput;
    passwordInput *passwordIn;
    ofxUILabel *usernameLabel, *passwordLabel, *emailLabel, *warning;
    
    CustomEventLabelButton * loginButton;
};