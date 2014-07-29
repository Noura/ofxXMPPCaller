//
//  passwordInput.h
//  example
//
//  Created by Catherine Liu on 7/24/14.
//
//

#ifndef PASSWORD_INPUT
#define PASSWORD_INPUT

#endif

#include <iostream>
#include "ofxUI.h"

class passwordInput: public ofxUITextInput{

public:

    passwordInput(string _name, string _textstring, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    //overwrite drawFill so that it only displays *** as the password? or recalculateDisplayString()?
    virtual void drawFill();
    //void keyPressed(int key);
    void recalculateDisplayString();
    
protected:
    //displaystring = ****
    //textstring = password?
};
