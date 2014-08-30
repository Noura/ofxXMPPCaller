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
    void setVisible(bool _visible);
    string getUsername();
    string getPassword();
    void setWarning(string warningMsg);
    void switchInputs();
    void trigger(bool &e);
    ofEvent<bool> inputSubmitted;
    void setBackgroundColor(ofColor color);
protected:
    
    float x, y, w, h;
    bool visible;
    ofxUICanvas * loginGUI;
    ofxUICanvas * sharedFonts;
    string user, password;
    ofxUITextInput *usernameInput;
    passwordInput *passwordIn;
    ofxUILabel *usernameLabel, *passwordLabel, *emailLabel, *warning;
    
    CustomEventLabelButton * loginButton;
};