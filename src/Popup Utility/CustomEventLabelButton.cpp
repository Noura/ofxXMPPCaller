/*
 * CustomEventLabelButton.h
 *
 *  Created on: Oct 7, 2013
 *      Author: noura
 */

#include "CustomEventLabelButton.h"

CustomEventLabelButton::CustomEventLabelButton(string _label, float w, float h, float x, float y, int _size)
: ofxUILabelButton(_label, false, w, h, x, y, _size) {
    ofAddListener(ofEvents().mousePressed, this, &CustomEventLabelButton::onMousePressed);
}

CustomEventLabelButton::~CustomEventLabelButton() {
    ofRemoveListener(ofEvents().mousePressed, this, &CustomEventLabelButton::onMousePressed);
}

void CustomEventLabelButton::onMousePressed(ofMouseEventArgs &e) {
    if (!rect->inside(e.x, e.y)) {
        return;
    }
    bool value = true;
    ofNotifyEvent(mousePressed, value, this);
}

void CustomEventLabelButton::triggerEvent() { }