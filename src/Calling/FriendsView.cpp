/*
 *  FriendsView.cpp
 *
 *  Manages the list of contacts from RTP
 *  ofxGstXMPPRTP is the model, FriendsView is the view
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#include "FriendsView.h"

FriendsView::FriendsView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, ofxXMPP * _xmpp, ofxUICanvas * _sharedFonts)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, legend_h(100.0)
, appState(_appState)
, xmpp(_xmpp)
, sharedFonts(_sharedFonts)
, canvas(NULL) {
}

FriendsView::~FriendsView() {
    ofRemoveListener(xmpp->userConnected, this, &FriendsView::addFriendView);
    ofRemoveListener(xmpp->userDisconnected, this, &FriendsView::removeFriendView);
    delete canvas;
}

void FriendsView::setup() {
    canvas = new dynamicListVerticalScrollbarCanvas(x, y, w, h - legend_h, sharedFonts);
    canvas->getScrollbar()->setImage("GUI/scrollbar.png");
    
    const vector<ofxXMPPUser> & friends = xmpp->getFriends();
    for (int i = 0; i < friends.size(); i++) {
        ofxXMPPUser user = friends[i];
        addFriendView(user);
    }

    ofAddListener(xmpp->userConnected, this, &FriendsView::addFriendView);
    ofAddListener(xmpp->userDisconnected, this, &FriendsView::removeFriendView);
}

// TODO also update friends' changing availability statuses

void FriendsView::addFriendView(ofxXMPPUser & user) {
    // this callback runs on the XMPP thread
    xmpp->lock();
    to_add.push_back(user);
    xmpp->unlock();
}

void FriendsView::removeFriendView(ofxXMPPUser & user) {
    // this callback runs on the XMPP thread
    xmpp->lock();
    to_remove.push_back(user);
    xmpp->unlock();
}

void FriendsView::update() {
    vector<ofxXMPPUser> to_add_copy;
    vector<ofxXMPPUser> to_remove_copy;
    xmpp->lock();
    while (!to_add.empty()) {
        to_add_copy.push_back(to_add.front());
        to_add.pop_front();
    }
    while (!to_remove.empty()) {
        to_remove_copy.push_back(to_remove.front());
        to_remove.pop_front();
    }
    xmpp->unlock();
    
    for (vector<ofxXMPPUser>::iterator it = to_add_copy.begin(); it < to_add_copy.end(); it++) {
        ofxXMPPUser user = (*it);
        list<ofxUIWidget*> * friends = canvas->getWidgetList();
        if (FriendView::isValidFriend(user)) {
            bool already_there = false;
            for (list<ofxUIWidget*>::iterator it = friends->begin(); it != friends->end(); it++) {
                FriendView * f = (FriendView*)(*it);
                if (f->user.userName == user.userName)
                    already_there = true;
            }
            if (!already_there) {
                FriendView * f = new FriendView(user, w - scroll_w, appState, xmpp);
                canvas->addWidget(f, false);
            }
        }
    }
    
    for (vector<ofxXMPPUser>::iterator uit = to_remove_copy.begin(); uit < to_remove_copy.end(); uit++) {
        ofxXMPPUser user = (*uit);
        list<ofxUIWidget*> * friends = canvas->getWidgetList();
        for (list<ofxUIWidget*>::iterator it = friends->begin(); it != friends->end(); it++) {
            FriendView * f = (FriendView*)(*it);
            if (f->user.userName == user.userName) {
                canvas->removeWidget(it, false);
                break;
            }
        }
    }
    
    if (to_add_copy.size() > 0 || to_remove_copy.size() > 0) {
        canvas->sortWidgets(&FriendView::comparator);
    }
    
    canvas->reflowWidgets();
    canvas->update();
}

void FriendsView::draw() {
    
    canvas->draw();
    
    FriendView::drawLegend(x + 5.0, y + h - legend_h + 5.0, appState);
}
