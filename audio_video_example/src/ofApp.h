/*
 * ofApp.h
 *
 *  Created on: Jul 19, 2013
 *      Author: arturo castro
 */

#pragma once

#include "ofMain.h"
#include "ofxGstXMPPRTP.h"
#include "ofxGui.h"
#include "ofxXMPPCaller.h"
#include "ofEvents.h"

class ofApp : public ofBaseApp{
    enum AppCurrentState{
        LOGIN_SCREEN,
        CALL_MANAGER,
        IN_CALL
    };
	public:
		void setup();
		void update();
		void draw();
		void exit();

		bool onFriendSelected(const void *sender);
		void onConnectionStateChanged(ofxXMPPConnectionState & connectionState);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void onCallReceived(string & from);
		void onCallAccepted(string & from);
		void onCallFinished(ofxXMPPTerminateReason & reason);

    bool proccessLoginInfo(bool &e);
    void setupRTP();
    void setupLoginScreen();
    void setupLogout();
    
    void setupCallButton();
    void sendCall(bool&e);
    
    void logout(bool&e);
		ofxGstXMPPRTP rtp;
		ofVideoGrabber grabber;
		ofTexture remoteVideo;
		int calling;
    string user, pass, server;
		//ofxPanel gui;
		//int guiState;
    int grabberWidth, grabberHeight, grabberX, grabberY;
    LoginUI* loginGUI;
    
    ofxUICanvas*logoutUI;
    CustomEventLabelButton* logoutButton;
    
    
    ofxUICanvas*callButtonUI;
    CustomEventLabelButton* callButton;
    
    ofxXMPPCaller * xmppCaller;
    AppCurrentState state;
};
