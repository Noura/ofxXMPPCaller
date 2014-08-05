/** Displays a single XMPP user contact
 *
 *  FriendView.h
 *
 *  Displays one RTP gchat contact
 *
 *  Created on: Sep 16, 2013
 *      Author: noura
 */

#ifndef example_all_FriendView_h
#define example_all_FriendView_h

#define FRIEND_STATE_AVAILABLE_COLOR 27, 224, 126
#define FRIEND_STATE_AWAY_COLOR 246, 246, 0
#define FRIEND_STATE_DND_COLOR 255, 150, 25
#define FRIEND_STATE_XA_COLOR 255, 50, 50

#define FRIEND_STATE_CIRCLE_RADIUS 5
#define FRIEND_STATE_CIRCLE_MARGIN 2

#define FRIEND_HEIGHT 30

#include "ofMain.h"
#include "ofxUI.h"
#include "SharedStateBundle.h"

class FriendView : public ofxUILabelButton {

public:
    
    FriendView(ofxXMPPUser _user, float _w, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp);
    ~FriendView();

    void draw();
    
    /** sets appState->chatContact to its user */
    void onMousePressed(ofMouseEventArgs &e);
    
    /** returns whether this user has the custom app capability */
    bool hasCapability();
    
    /** translates the ofxXMPPShow enum status of this user into an integer where lower means more available */
    int status_rank();
    
    /** checks to make sure _user would be a valid input to the FriendView constructor */
    static bool isValidFriend(const ofxXMPPUser &_user);
    
    /** format an ofxXMPPUser user.userName in a consistent way */
    static string formatUserName(string _name);
    
    /** drawing user availability icons and a legend to explain the icons */
    static void drawLegend(float x, float y, SharedStateBundle * _appState);
    static void drawAvailableIcon(float x, float y);
    static void drawAwayIcon(float x, float y);
    static void drawDndIcon(float x, float y);
    static void drawXAIcon(float x, float y);
    static void drawCapabilityIcon(float x, float y);
    
    /** sorts FriendView objects by whether they have the custom app capability, then by availability, then alphabetically */
    static bool comparator(const ofxUIWidget * lhs, const ofxUIWidget * rhs);
    
    ofxXMPPUser user;
    
protected:

    SharedStateBundle * appState;
    shared_ptr<ofxXMPP> xmpp;
    
};

#endif
