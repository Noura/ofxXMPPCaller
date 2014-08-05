/*
 *  Messages.cpp
 *
 *  Manages the friend-to-friend chat messages from RTP
 *  Messages is the model, MessagesView is the view
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#include "Messages.h"
#include "MessagesView.h"

Messages::Messages(SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp)
: appState(_appState)
, xmpp(_xmpp)
, view(NULL) {
}

Messages::~Messages() {
    if (view) ofRemoveListener(view->newLocalMessage, this, &Messages::onNewLocalMessage);
}

void Messages::setView(MessagesView * view) {
    if (view) ofRemoveListener(view->newLocalMessage, this, &Messages::onNewLocalMessage);
    ofAddListener(view->newLocalMessage, this, &Messages::onNewLocalMessage);
}

void Messages::addMessage(ofxXMPPMessage & msg){
	messages.push_back(msg);
	if (messages.size() > MESSAGES_MAX_NUM) messages.pop_front();
    ofNotifyEvent(newMessage, msg, this);
}

void Messages::onNewLocalMessage(string & msg) {
    ofxXMPPMessage message;
    message.body = msg.substr();
    message.from = "me";
    addMessage(message);

    xmpp->sendMessage(appState->chatContact.userName, message.body);
}