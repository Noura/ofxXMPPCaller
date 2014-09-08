/** Displays the list of contacts from XMPP
 *
 *  FriendsView.h
 *
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#ifndef example_all_FriendsView_h
#define example_all_FriendsView_h

class CallingGUI;

#include "ofMain.h"
#include "ofxUI.h"

#include "SharedStateBundle.h"
#include "FriendView.h"
#include "dynamicListVerticalScrollbarCanvas.h"

class FriendsView {
    
public:
    
    FriendsView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp, ofxUICanvas * _sharedFonts);
    ~FriendsView();

    void addFriendView(ofxXMPPUser & user);
    void removeFriendView(ofxXMPPUser & user);
    void setup();
    void update();
    void draw();
    void setVisible(bool _visible);
    void clearFriends();
    void setDisplayCapable(bool _display);

private:
    
    SharedStateBundle * appState;
    shared_ptr<ofxXMPP> xmpp;
    bool visible;
    bool displayCapable;
    float x, y, w, h, legend_h;
    float scroll_w = 30;
    dynamicListVerticalScrollbarCanvas * canvas;
    deque<ofxXMPPUser> to_remove;
    deque<ofxXMPPUser> to_add;
    
    ofxUICanvas * sharedFonts;
};

#endif