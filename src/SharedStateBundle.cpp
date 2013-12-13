/*
 * SharedStateBundle.cpp
 *
 * Application state that is shared across multiple classes should be stored
 * here. Member variables are public for convenient reading, but for writing
 * the setter methods should always be used because they trigger events. Many
 * parts of the application can listen for changes to the shared appState
 * object.
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#include "SharedStateBundle.h"

SharedStateBundle::SharedStateBundle()
: callCapability("") {
    chatContact.userName = "";
}

void SharedStateBundle::setChatContact(ofxXMPPUser _user) {
    chatContact = _user;
    ofNotifyEvent(chatContactChange, chatContact, this);
}

void SharedStateBundle::setCallCapability(string _callCapability) {
    callCapability = _callCapability;
    ofNotifyEvent(callCapabilityChange, callCapability, this);
}
