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
class LoginUI {
    
public:
    
    LoginUI(float _x, float _y,  float _w, float _h, SharedStateBundle * _appState,ofxUICanvas * _sharedFonts);
    ~LoginUI();
    
    void setup();
    void update();
    void draw();
    
    string getUsername();
    string getPassword();
    void setWarning(string warningMsg);
    ofEvent<bool> inputSubmitted;
    
protected:
    
    float x, y, w, h;
    
    SharedStateBundle * appState;
    
    
    void trigger(bool &b);
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas * loginGUI;
    ofxUICanvas * sharedFonts;
    string user, password;
    ofxUITextInput *usernameInput, *passwordInput;
    ofxUILabel *usernameLabel, *passwordLabel, *emailLabel, *warning;
    
    CustomEventLabelButton * loginButton;
};