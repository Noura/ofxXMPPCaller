/*
 * SharedStateBundle.h
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

#ifndef example_all_AppState_h
#define example_all_AppState_h

#include "ofxXMPP.h"

class SharedStateBundle {
    
public:
    
    SharedStateBundle();
    
    ofxXMPPUser chatContact;
    ofEvent<ofxXMPPUser> chatContactChange;
    void setChatContact(ofxXMPPUser _user);
    
    string callCapability;
    ofEvent<string> callCapabilityChange;
    void setCallCapability(string _callCapability);

};

#endif
