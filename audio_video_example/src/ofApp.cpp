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
    uiLock.lock();
    setupLoginScreen();
    uiLock.unlock();
    ring.loadSound("ring.wav",false);
	callingState = Disconnected;
	calling = -1;
	lastRing = 0;
    
    sharedResources = new ofxUICanvas(0,0,0,0);
    sharedResources->setFont("GUI/NewMediaFett.ttf");
    
    xmpp = shared_ptr<ofxXMPP>(new ofxXMPP);
    
    xmpp->setCapabilities("telekinect");
    xmpp->setShow(ofxXMPPShowAvailable);
    
    rtp.setXMPP(xmpp);
}
//--------------Setup functions---------------------------------
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

void ofApp::setupCallButton(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = grabberX + grabberWidth/2-unlaunchW/2;
    float unlaunchY = grabberY+grabberHeight+10;
    
    callButtonUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH, sharedResources);
    
    callButton = new CustomEventLabelButton("Start Call", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    callButtonUI->addWidget(callButton);
    
    ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
    
    callButtonUI->setColorBack(ofColor(150));
}

void ofApp::setupLogout(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = ofGetWidth() - unlaunchW;
    float unlaunchY = 0;
    
    // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
    logoutUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH, sharedResources);
    logoutButton = new CustomEventLabelButton("Logout", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    logoutUI->addWidget(logoutButton);
    
    ofAddListener(logoutButton->mousePressed, this, &ofApp::logout);
    
    ofColor dark(80);
    logoutUI->setColorBack(dark);
}

void ofApp::setupCallManager(){
    
	rtp.setup(500);
    //xmpp is connected on xmppCaller->setup();
	//rtp.getXMPP().connect(server, user, pass);
    rtp.setStunServer("132.177.123.6");
	rtp.addSendVideoChannel(640,480,30);
	rtp.addSendAudioChannel();
    
	calling = -1;
    
	//ofBackground(255);
    
	ofAddListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofAddListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofAddListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    
    //ofBackground(ofColor(OFX_UI_COLOR_BACK_ALPHA));
    //move video to mid right
    grabberX = 1000-grabberWidth-50;
    //Chat notification height is 180, notification y = 50
    grabberY = 50+180+10;
    
    cout<<"\n\n"<<user<<" "<<pass<<"\n\n";
    xmppCaller = new ofxXMPPCaller(0,0, server, user, pass, "Login", "telekinect", xmpp, sharedResources);
    xmppCaller->setup();
    
    setupCallButton();
    
    state = CALL_MANAGER;
    if(loginGUI){
        delete loginGUI;
        loginGUI = NULL;
    }
    
    setupLogout();
    
}

void ofApp::setupEndCallButton(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = ofGetWidth() - unlaunchW;
    float unlaunchY = 0;
    
    endCallUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH, sharedResources);
    endCallButton = new CustomEventLabelButton("End Call", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    endCallUI->addWidget(endCallButton);
    
    ofAddListener(endCallButton->mousePressed, this, &ofApp::endCall);
    
    ofColor dark(80);
    endCallUI->setColorBack(dark);
}

//--------------Event Callbacks---------------------------------
void ofApp::endCall(bool &e){
    ofxXMPPTerminateReason reason = ofxXMPPTerminateSuccess;
    rtp.endCall();
    onCallFinished(reason);
}

bool ofApp::proccessLoginInfo(bool &e){
    string u = loginGUI->getUsername();
    string p = loginGUI->getPassword();
    
    if (u.length()<=0) {
        if (p.length()<=0){
            uiLock.lock();
            loginGUI->setWarning("Enter a username and password!");
            uiLock.unlock();
        }
        else{
            uiLock.lock();
            loginGUI->setWarning("Enter a username!");
            uiLock.unlock();
        }
        
        return false;
    }
    else if (p.length()<=0){
        uiLock.lock();
        loginGUI->setWarning("Enter a password!");
        uiLock.unlock();
        return false;
        
    }
    else{
        user = u+"@gmail.com";
        pass = p;
        setupCallManager();
        return true;
    }
    
}

void ofApp::onCallingDialogAnswer(bool & _answer) {
    
    /*
     //you are calling and you want to end your call
     
     if(callingState == Calling && _answer){
     cout<<"TRYING TO END CALL";
     //rtp.endCall();
     rtp.refuseCall();
     
     // reset the rtp element to be able to start a new call
     rtp.setup(200);
     rtp.setStunServer("132.177.123.6");
     rtp.addSendVideoChannel(640,480,30);
     rtp.addSendAudioChannel();
     
     // reset the state
     callingState = Disconnected;
     calling = -1;
     state = CALL_MANAGER;
     }*/
    if(callingState == ReceivingCall){
        cout<<"\n\nanswering dialog\n\n";
        ofRemoveListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
        delete callDialog;
        if(_answer){
			rtp.acceptCall();
            
			callingState = InCall;
            state = IN_CALL;
            
            uiLock.lock();
            setupEndCallButton();
            delete xmppCaller;
            xmppCaller = NULL;
            delete callButtonUI;
            delete logoutUI;
            uiLock.unlock();
            
            grabberX = 1000-grabberWidth;
            grabberY = 700-grabberHeight;
            
        }else{
            //you reject the call
			rtp.refuseCall();
            callingState=Disconnected;
            
            // reset the rtp element to be able to start a new call
            rtp.setup(200);
            rtp.setStunServer("132.177.123.6");
            rtp.addSendVideoChannel(640,480,30);
            rtp.addSendAudioChannel();
        }
        
    }
    
}

void ofApp::logout(bool &e){
    uiLock.lock();
    delete xmppCaller;
    xmppCaller = NULL;
    
    ofRemoveListener(logoutButton->mousePressed, this, &ofApp::logout);
    delete logoutUI;
    
    ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
    delete callButtonUI;
    uiLock.unlock();
    
    ofRemoveListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofRemoveListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofRemoveListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    //TODO fix this so that there is only 1 xmpp thread
    rtp.getXMPP().stop();
    
    setupLoginScreen();
}

void ofApp::sendCall(bool &e){
    ofxXMPPUser receiver = xmppCaller->getAppState().chatContact;
    for(int i = 0; i<receiver.capabilities.size();i++){
        if (receiver.capabilities[i]=="telekinect") {
            
            ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
            
            uiLock.lock();
            delete callButtonUI;
            string notification = "You are currently calling "+receiver.userName+".";
            callNotification = new ofxUICanvas(700, 700-180-10, 300, 180, sharedResources);
            callNotification->addTextArea("msg", notification);
            uiLock.unlock();
            
            rtp.call(receiver);
            callingState = Calling;
            
            /*string dialog = "You are currently calling "+receiver.userName+". Would you like to stop the call?";
             callDialog = new YesNoDialog(700, 700-180-10, 300, 180, dialog, sharedResources);
             callDialog->setup();
             ofAddListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
             */
        }
    }
}

// will be called whenever there's a new call from another peer
void ofApp::onCallReceived(string & from){
	callFrom = ofSplitString(from,"/")[0];
    string dialog = "You are currently being called by "+callFrom+". Would you like to accept the call?";
    cout<<"\n\n"<<dialog<<"\n\n";
    
    uiLock.lock();
    callDialog = new YesNoDialog(700, 700-180-10, 300, 180, dialog, sharedResources);
    callDialog->setup();
    ofAddListener(callDialog->answer, this, &ofApp::onCallingDialogAnswer);
    uiLock.unlock();
    
	callingState = ReceivingCall;
}

// will be called when we start a call and the other peer accepts it
void ofApp::onCallAccepted(string & from){
    
	if(callingState == Calling){
        
        grabberX = 1000-grabberWidth;
        grabberY = 700-grabberHeight;
        
        uiLock.lock();
        setupEndCallButton();
        delete callNotification;
        cout<<"deleting xmppCaller";
        delete xmppCaller;
        xmppCaller = NULL;
        delete logoutUI;
        cout<<"finished deleting xmppCaller";
        uiLock.unlock();
        
        
        callingState = InCall;
        state = IN_CALL;
	}
}

// will be called whenever the call ends and
// receives the reason as parameter
void ofApp::onCallFinished(ofxXMPPTerminateReason & reason){
    
	cout << "received end call" << endl;
	if(callingState==Calling){
		// if we started a call most likely the other end declined it
		// or the call failed
		ofSystemAlertDialog("Call declined");
        
        uiLock.lock();
        delete callNotification;
        setupCallButton();
        uiLock.unlock();
        
        //reset states
        callingState = Disconnected;
        calling = -1;
        // reset the rtp element to be able to start a new call
        rtp.setup(200);
        rtp.setStunServer("132.177.123.6");
        rtp.addSendVideoChannel(640,480,30);
        rtp.addSendAudioChannel();
        
        
    }
    else{
        //reset states
        callingState = Disconnected;
        calling = -1;
        
        uiLock.lock();
        delete endCallUI;
        setupCallManager();
        uiLock.unlock();
        state = CALL_MANAGER;
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
	grabber.update();
    uiLock.lock();
    if(state == LOGIN_SCREEN)
        loginGUI->update();
    else if(state==IN_CALL){
        
        //send your new frame
        if(grabber.isFrameNew()){
            rtp.getServer().newFrame(grabber.getPixelsRef());
        }
        //update remote video/audio/etc
        rtp.getClient().update();
        if(rtp.getClient().isFrameNewVideo()){
            remoteVideo.loadData(rtp.getClient().getPixelsVideo());
        }
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
    uiLock.unlock();
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    uiLock.lock();
    if(state==LOGIN_SCREEN)
        loginGUI->draw();
    else if(state == CALL_MANAGER){
        if(xmppCaller){
            xmppCaller->draw();
            
        }
        //logoutUI->draw();
        if(callingState == Disconnected){
            callButtonUI->draw();
        }
        if(callingState==ReceivingCall){
            callDialog->draw();
        }
        if(callingState==Calling){
            callNotification->draw();
        }
    }
    else if(state == IN_CALL){
        remoteVideo.draw(0,0, 1000, 700);
        endCallUI->draw();
        
    }
    uiLock.unlock();
	grabber.draw(grabberX,grabberY,grabberWidth,grabberHeight);
}

//--------------------------------------------------------------
void ofApp::exit(){
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

