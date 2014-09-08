/*
 *  MessagesView.h
 *
 *  Manages the friend-to-friend chat messages from RTP
 *  Messages is the model, MessagesView is the view
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#ifndef example_all_MessagesView_h
#define example_all_MessagesView_h

#include "ofMain.h"
#include "ofxUI.h"

#include "dynamicListVerticalScrollbarCanvas.h"
#include "multiLineTextInput.h"
#include "SharedStateBundle.h"
#include "Messages.h"
#include "FriendView.h"

#define CONVERSATION_PERCENT_HEIGHT 70.0

class Messages;

class MessagesView {
  
public:
    
    MessagesView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp);
    MessagesView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp, ofxUICanvas * _sharedResources);
    ~MessagesView();
    void setModel(Messages * _model);

    void setup();
    void update();
    void draw();
    void setVisible(bool _visible);
    
    static string formatMessage(ofxXMPPMessage msg);
    void addMessage(ofxXMPPMessage &msg);
    void onNewLocalMessage(string &msg);
    ofEvent<string> newLocalMessage;
    
private:
    ofxUICanvas * sharedResources;
    SharedStateBundle * appState;
    shared_ptr<ofxXMPP> xmpp;
    Messages * model;
    
    bool visible;
    float x, y, w, h, canvas_h, title_h;
    string title;
    dynamicListVerticalScrollbarCanvas * messagesCanvas;
    ofxUICanvas * composingCanvas;
    ofxUICanvas * chatLabel;
    ofxUILabel * label;
    multiLineTextInput * composingMsg;

};

#endif
