/*
 * CustomEventLabelButton.cpp
 *
 * An ofxUILabelButton that does not trigger events that propagate
 * up to its canvas, but instead fires its own events directly.
 *
 * This can be helpful if you need to circumvent the way ofxUICanvas
 * handles events.
 *
 *  Created on: Oct 7, 2013
 *      Author: noura
 */

#ifndef CUSTOM_EVENT_LABEL_BUTTON_H_
#define CUSTOM_EVENT_LABEL_BUTTON_H_

#include <iostream>
#include "ofxUI.h"

class CustomEventLabelButton : public ofxUILabelButton {
public:
    
    CustomEventLabelButton(string _label, float w, float h, float x, float y, int size_ = OFX_UI_FONT_MEDIUM);
    
    ~CustomEventLabelButton();
    
    void triggerEvent();
    
    void onMousePressed(ofMouseEventArgs & e);
    ofEvent<bool> mousePressed; // the bool value will always be true, this event is just saying the button got clicked
    void setVisible(bool _visible);
private:
    bool visible;
};

#endif /* defined(CUSTOM_EVENT_LABEL_BUTTON_H_) */
