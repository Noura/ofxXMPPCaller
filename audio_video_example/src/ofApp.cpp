#include "ofApp.h"
#include "ofxNice.h"
#include "ofxGStreamer.h"
//#define BUNDLED

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef BUNDLED
    ofxGStreamerSetBundleEnvironment();
#endif
    
    server = "talk.google.com";
    
    grabber.initGrabber(640,480);
	remoteVideo.allocate(640,480,GL_RGB);

    setupLoginScreen();
    
    ring.loadSound("ring.wav",false);
	callingState = Disconnected;
	calling = -1;
	lastRing = 0;

    //ofSetLogLevel(ofxGstRTPServer::LOG_NAME,OF_LOG_VERBOSE);
	//ofSetLogLevel(ofxGstRTPClient::LOG_NAME,OF_LOG_VERBOSE);
    //grabber.setDeviceID(1);
	
    
    
}

void ofApp::setupLoginScreen(){
    state = LOGIN_SCREEN;
    
    //set the color to match the loginUI's background
    ofBackground(ofColor(OFX_UI_COLOR_BACK_ALPHA));
    int appWidth = 1000;
    int appHeight =700;
    int loginWidth = 500;
    int loginHeight = 180;
    
    //your video preview
    grabberWidth = 240;
    grabberHeight = 180;
    
    //Math to center the video and login screen
    
    int totalHeight = grabberHeight+loginHeight;
    grabberX = appWidth/2-grabberWidth/2;
    grabberY = appHeight/2-totalHeight/2;
    int loginX = appWidth/2-loginWidth/2;
    int loginY = grabberY+grabberHeight;
    loginGUI = new LoginUI(loginX, loginY, loginWidth,loginHeight);
    ofAddListener(loginGUI->inputSubmitted, this, &ofApp::proccessLoginInfo);
}

void ofApp::setupLogout(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = ofGetWidth() - unlaunchW;
    float unlaunchY = 0;
    
    // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
    logoutUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH);
    logoutUI->setFont("GUI/NewMediaFett.ttf");
    logoutButton = new CustomEventLabelButton("Logout", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    logoutUI->addWidget(logoutButton);
    
    ofAddListener(logoutButton->mousePressed, this, &ofApp::logout);
    
    ofColor dark(80);
    logoutUI->setColorBack(dark);
}
void ofApp::logout(bool &e){
    
    delete xmppCaller;
    
    ofRemoveListener(logoutButton->mousePressed, this, &ofApp::logout);
    delete logoutUI;
    
    ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
    delete callButtonUI;
    
    ofRemoveListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofRemoveListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofRemoveListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    //TODO fix this so that there is only 1 xmpp thread
    rtp.getXMPP().stop();
    
    setupLoginScreen();
}

void ofApp::setupCallButton(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = grabberX + grabberWidth/2-unlaunchW/2;
    float unlaunchY = grabberY+grabberHeight+10;
    
    // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
    callButtonUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH);
    callButtonUI->setFont("GUI/NewMediaFett.ttf");
    callButton = new CustomEventLabelButton("Start Call", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    callButtonUI->addWidget(callButton);
    
    ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
    
    callButtonUI->setColorBack(ofColor(150));
}

void ofApp::sendCall(bool &e){
    ofxXMPPUser receiver = xmppCaller->getAppState().chatContact;
    for(int i = 0; i<receiver.capabilities.size();i++){
        if (receiver.capabilities[i]=="telekinect") {
            rtp.call(receiver);
            callingState = Calling;
            string dialog = "You are currently calling "+receiver.userName+". Would you like to stop the call?";
            callDialog = new YesNoDialog(700, 700-180-10, 300, 180, dialog);
            callDialog->setup();
            ofAddListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
        }
    }
}

// will be called whenever there's a new call from another peer
void ofApp::onCallReceived(string & from){
	callFrom = ofSplitString(from,"/")[0];
    string dialog = "You are currently being called by "+callFrom+". Would you like to accept the call?";
    cout<<"\n\n"<<dialog<<"\n\n";
    callDialog = new YesNoDialog(700, 700-180-10, 300, 180, dialog);
    callDialog->setup();
    ofAddListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
    
	callingState = ReceivingCall;
}

// will be called when we start a call and the other peer accepts it
void ofApp::onCallAccepted(string & from){
	if(callingState == Calling){
		callingState = InCall;
        state = IN_CALL;
        //TODO move the grabber's X/Y to bottom left of the remote video
        //Get rid of xmppCaller contacts view
        //if possible keep the chat and move to right side
	}
}

// will be called whenever the call ends and
// receives the reason as parameter
void ofApp::onCallFinished(ofxXMPPTerminateReason & reason){
	if(callingState==Calling){
		// if we started a call most likely the other end declined it
		// or the call failed
		ofSystemAlertDialog("Call declined");
        //ofRemoveListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
        delete callDialog;
	}
	cout << "received end call" << endl;
	// reset the rtp element to be able to start a new call
	rtp.setup(200);
	rtp.setStunServer("132.177.123.6");
	rtp.addSendVideoChannel(640,480,30);
	rtp.addSendAudioChannel();
    
	// reset the state
	callingState = Disconnected;
	calling = -1;
    
    state = CALL_MANAGER;
}

void ofApp::exit(){
}


//--------------------------------------------------------------
void ofApp::update(){
    if(state == LOGIN_SCREEN)
        loginGUI->update();
    else if(state==IN_CALL){
        //TODO update remote video, audio????
        /*
         // While in call, send your video
         if(grabber.isFrameNew()){
         rtp.getServer().newFrame(grabber.getPixelsRef());
         }
         //pull the remote video changes?
         rtp.getClient().update();
         //update your view of the remote video
         if(rtp.getClient().isFrameNewVideo()){
         remoteVideo.loadData(rtp.getClient().getPixelsVideo());
         }*/
    }
    else if(state==CALL_MANAGER){
        xmppCaller->update();
        if(callingState==ReceivingCall || callingState==Calling){
            unsigned long long now = ofGetElapsedTimeMillis();
            if(now - lastRing>2500){
                lastRing = now;
                ring.play();
            }
        }
    }
    
	grabber.update();
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(state==LOGIN_SCREEN)
        loginGUI->draw();
    else if(state == CALL_MANAGER){
        xmppCaller->draw();
        //logoutUI->draw();
        callButtonUI->draw();
        if(callingState==Calling || callingState==ReceivingCall){
            callDialog->draw();
        }
    }
    else if(state == IN_CALL){
        //TODO Draw remote video
        
    }
    
    
	grabber.draw(grabberX,grabberY,grabberWidth,grabberHeight);
    /*
     ofSetColor(255);
     remoteVideo.draw(0,0);
     grabber.draw(400,300,240,180);
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    /*
     if(calling==-1){
     ofVec2f mouse(x,y);
     ofRectangle friendsRect(ofGetWidth()-300,0,300,rtp.getXMPP().getFriends().size()*20);
     if(friendsRect.inside(mouse)){
     calling = mouse.y/20;
     rtp.call(rtp.getXMPP().getFriends()[calling]);
     }
     }*/
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

bool ofApp::proccessLoginInfo(bool &e){
    string u = loginGUI->getUsername();
    string p = loginGUI->getPassword();
    
    if (u.length()<=0) {
        if (p.length()<=0){
            
            loginGUI->setWarning("Enter a username and password!");
            
        }
        else
            loginGUI->setWarning("Enter a username!");
        return false;
    }
    else if (p.length()<=0){
        
        loginGUI->setWarning("Enter a password!");
        return false;
        
    }
    else{
        user = u+"@gmail.com";
        pass = p;
        setupRTP();
        return true;
    }
    
}

void ofApp::setupRTP(){
    
    state = CALL_MANAGER;
    delete loginGUI;
    
	//ofxNiceEnableDebug();
    
    shared_ptr<ofxXMPP> xmpp = shared_ptr<ofxXMPP>(new ofxXMPP);
    
    xmpp->setCapabilities("telekinect");
    xmpp->setShow(ofxXMPPShowAvailable);
    
    rtp.setXMPP(xmpp);
    
	rtp.setup(500);
	//rtp.getXMPP().connect(server, user, pass);
    rtp.setStunServer("132.177.123.6");
	rtp.addSendVideoChannel(640,480,30);
	rtp.addSendAudioChannel();
    
	calling = -1;
    
	ofBackground(255);
    
	ofAddListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofAddListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofAddListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
   
    ofBackground(ofColor(OFX_UI_COLOR_BACK_ALPHA));
    //move video to mid right
    grabberX = 1000-grabberWidth-50;
    //Chat notification height is 180, notification y = 50
    grabberY = 50+180+10;
    
    xmppCaller = new ofxXMPPCaller(0,0, server, user, pass, "Login", "telekinect", xmpp);
    xmppCaller->setup();
    
    setupCallButton();
    //setupLogout();
    
}

void ofApp::onCallingDialogAnswer(bool & _answer) {
    cout<<"\n\nanswering dialog\n\n";
    ofRemoveListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
    delete callDialog;
    
    if(callingState == Calling && _answer){
        //you are calling and you want to end your call
        
        rtp.endCall();
        // reset the rtp element to be able to start a new call
        rtp.setup(200);
        rtp.setStunServer("132.177.123.6");
        rtp.addSendVideoChannel(640,480,30);
        rtp.addSendAudioChannel();
        
        // reset the state
        callingState = Disconnected;
        calling = -1;
        state = CALL_MANAGER;
    }
    if(callingState == ReceivingCall){
        if(_answer){
			rtp.acceptCall();
			callingState = InCall;
            state = IN_CALL;
            //TODO set up call interface
            
        }else{
            //you reject the call
			rtp.refuseCall();
            callingState=Disconnected;
        }
        
    }
    
}
