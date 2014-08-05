/*
 * YesNoDialog.h
 *
 *  Created on: Oct 7, 2013
 *      Author: noura
 */

#ifndef YESNODIALOG_H_
#define YESNODIALOG_H_

#include <iostream>
#include "ofMain.h"
#include "ofxUI.h"
#include "CustomEventLabelButton.h"

class YesNoDialog {
public:
    
    YesNoDialog(float _x, float _y, float _w, float _h, std::string _msg);
    YesNoDialog(float _x, float _y, float _w, float _h, std::string _msg, ofxUICanvas * _sharedResources);
    
    ~YesNoDialog();
    
    void Yes(bool & val);
    void No(bool & val);
    
    ofEvent<bool> answer;
    
    void setup();
    void draw();
    
private:
    
    float x, y, w, h;
    string msg;
    ofxUICanvas * canvas;
    ofxUICanvas * sharedResources;
    ofxUITextArea * prompt;
    CustomEventLabelButton * yesButton, * noButton;
    
};

#endif /* defined(YESNODIALOG_H_) */
