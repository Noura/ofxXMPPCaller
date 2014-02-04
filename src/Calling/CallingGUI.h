/*
 * CallingGUI.h
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#ifndef example_all_CALLING_GUI_H
#define example_all_CALLING_GUI_H

#include "SharedStateBundle.h"
#include "ofxUI.h"
#include "Messages.h"
#include "MessagesView.h"
#include "FriendsView.h"
#include "YesNoDialog.h"

#define CALLING_GUI_FRIENDS_WIDTH 300
#define CALLING_GUI_MESSAGES_WIDTH 300
#define CALLING_GUI_BORDER_WIDTH 10

class CallingGUI {

public:

    CallingGUI(float _x, float _y, SharedStateBundle * _appState, ofxXMPP * _xmpp, ofxUICanvas * _sharedFonts);
    ~CallingGUI();
    
    void setup();
    void update();
    void draw();
    
    void onChatContactChange(ofxXMPPUser & _user);
    void onNewRemoteMessage(ofxXMPPMessage & _msg);
    void onCallingDialogAnswer(bool & _answer);
    
    static bool isSameXMPPUserName(string userName1, string userName2);
    
protected:
    
    float x, y;
    
    SharedStateBundle * appState;
    ofxXMPP * xmpp;

    FriendsView * friendsView;
    
    Messages * messages;
    MessagesView * messagesView;
    
    YesNoDialog * callingDialog;
    ofxXMPPMessage tempMsg;
    bool addTempMsg;
    
    ofxUICanvas * sharedFonts;
    
};


#endif
