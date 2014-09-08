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
    
    sharedResources = new ofxUICanvas(0,0,0,0);
    sharedResources->setFont("GUI/NewMediaFett.ttf");
    
    grabber.initGrabber(640,480);
	remoteVideo.allocate(640,480,GL_RGB);
    uiLock.lock();
    setupUtilities();
    setupLoginScreen();
    uiLock.unlock();
    ring.loadSound("ring.wav",false);
	callingState = Disconnected;
	calling = -1;
	lastRing = 0;
    
    xmpp = shared_ptr<ofxXMPP>(new ofxXMPP);
    
    xmpp->setCapabilities("telekinect");
    xmpp->setShow(ofxXMPPShowAvailable);
    
    rtp.setXMPP(xmpp);
}
//--------------Setup functions---------------------------------
void ofApp::setupLoginScreen(){
    state = LOGIN_SCREEN;
    utilities->setPosition(ofGetWidth()-utilities->getRect()->getWidth(), 0);
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
void ofApp::setupUtilities(){
    float h = 40;
    float margin = 4;
    float w = h*4+7*margin;
    utilities = new ofxUICanvas(ofGetWidth()-w, 0, w, h+2*margin, sharedResources);
    callManagerImage = new ofxUIImageButton(h, h, true, "GUI/callManager.png", "CM");
    helpImage = new ofxUIImageButton(h, h, true, "GUI/help.png", "HELP");
    sceneMakerImage = new ofxUIImageButton(h, h, true, "GUI/sceneMaker.png", "SM");
    settingsImage = new ofxUIImageButton(h, h, true, "GUI/settings.png", "SETTINGS");
    utilities->addWidgetPosition(callManagerImage);
    utilities->addWidgetEastOf(helpImage, "CM");
    utilities->addWidgetEastOf(sceneMakerImage, "HELP");
    utilities->addWidgetEastOf(settingsImage, "SM");
    
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
    float unlaunchH = utilities->getRect()->getHeight();
    float margin = 3;
    float unlaunchX = ofGetWidth() - unlaunchW;
    float unlaunchY = 0;
    int w = utilities->getRect()->getWidth();
    utilities->setPosition(ofGetWidth()-75-w, 0);
    
    logoutUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH, sharedResources);
    logoutButton = new CustomEventLabelButton("Logout", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    logoutUI->addWidget(logoutButton);
    ofAddListener(logoutButton->mousePressed, this, &ofApp::logout);
    
    ofColor dark(80);
    logoutUI->setColorBack(dark);
}

void ofApp::setupCallManager(){
    
	rtp.setup(50);
    //xmpp is connected on xmppCaller->setup();
    rtp.setStunServer("132.177.123.6");
	rtp.addSendVideoChannel(640,480,30);
	rtp.addSendAudioChannel();
    
	calling = -1;
    
    
	ofAddListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofAddListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofAddListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    
    //move video to mid right
    grabberX = 1000-grabberWidth-50;
    //Chat notification height is 180, notification y = 50
    grabberY = 50+180+10;
    
    state = CALL_MANAGER;
    if(loginGUI){
        loginGUI->setVisible(false);
        ofRemoveListener(loginGUI->inputSubmitted, this, &ofApp::proccessLoginInfo);
    }
    
    xmppCaller = new ofxXMPPCaller(0,0, server, user, pass, "Login", "telekinect", xmpp, sharedResources);
    xmppCaller->setup();
    xmppCaller->setDisplayCapable(true);
    setupCallButton();
    setupLogout();
    
}

void ofApp::setupEndCallButton(){
    float unlaunchW = 75;
    float unlaunchH = utilities->getRect()->getHeight();
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
        if(xmppCaller){
            xmppCaller->setVisible(true);
            rtp.setup(50);
            rtp.setStunServer("132.177.123.6");
            rtp.addSendVideoChannel(640,480,30);
            rtp.addSendAudioChannel();
            cout<<"\n\n";
            cout<<user<<pass;
            cout<<"\n\n";
            //TO DO try to check that it connects and do a warning
            xmpp->connect("talk.google.com", user, pass);
            
            calling = -1;
            
            
            ofAddListener(rtp.callReceived,this,&ofApp::onCallReceived);
            ofAddListener(rtp.callFinished,this,&ofApp::onCallFinished);
            ofAddListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
            
            //move video to mid right
            grabberX = 1000-grabberWidth-50;
            //Chat notification height is 180, notification y = 50
            grabberY = 50+180+10;
            
            callButtonUI->setVisible(true);
            ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
            state = CALL_MANAGER;
            if(loginGUI){
                loginGUI->setVisible(false);
                ofRemoveListener(loginGUI->inputSubmitted, this, &ofApp::proccessLoginInfo);
            }
            if(logoutUI){
                logoutUI->setVisible(true);
                ofAddListener(logoutButton->mousePressed, this, &ofApp::logout);
            }
            utilities->setPosition(ofGetWidth()-75-utilities->getRect()->getWidth(), 0);
            
        }
        else{
            setupCallManager();
        }
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
            if(endCallUI){
                endCallUI->setVisible(true);
                ofAddListener(endCallButton->mousePressed, this, &ofApp::endCall);
            }
            else
                setupEndCallButton();
            xmppCaller->setVisible(false);
            callButtonUI->setVisible(false);
            ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
            
            logoutUI->setVisible(false);
            uiLock.unlock();
            
            grabberX = 1000-grabberWidth;
            grabberY = 700-grabberHeight;
            
        }else{
            //you reject the call
			rtp.refuseCall();
            callingState=Disconnected;
            
            ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
            
            // reset the rtp element to be able to start a new call
            rtp.setup(50);
            rtp.setStunServer("132.177.123.6");
            rtp.addSendVideoChannel(640,480,30);
            rtp.addSendAudioChannel();
        }
        
    }
    
}

void ofApp::logout(bool &e){
    uiLock.lock();
    xmppCaller->setVisible(false);
    ofRemoveListener(rtp.callReceived,this,&ofApp::onCallReceived);
    ofRemoveListener(rtp.callFinished,this,&ofApp::onCallFinished);
    ofRemoveListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    
    ofRemoveListener(logoutButton->mousePressed, this, &ofApp::logout);
    logoutUI->setVisible(false);
    
    ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
    callButtonUI->setVisible(false);
    
    //recenter video
    grabberWidth = 240;
    grabberHeight = 180;
    int totalHeight = grabberHeight+180;
    grabberX = ofGetWidth()/2-grabberWidth/2;
    grabberY = ofGetHeight()/2-totalHeight/2;
    
    utilities->setPosition(ofGetWidth()-utilities->getRect()->getWidth(), 0);
    uiLock.unlock();
    
    xmppCaller->deleteMessagesFriends();
    if(rtp.getXMPP().getConnectionState()!=ofxXMPPDisconnected)
        rtp.getXMPP().stop();
    
    loginGUI->setVisible(true);
    ofAddListener(loginGUI->inputSubmitted, this, &ofApp::proccessLoginInfo);
    state = LOGIN_SCREEN;
}

void ofApp::sendCall(bool &e){
    bool success = false;
    ofxXMPPUser receiver = xmppCaller->getAppState().chatContact;
    for(int i = 0; i<receiver.capabilities.size();i++){
        if (receiver.capabilities[i]=="telekinect") {
            
            ofRemoveListener(callButton->mousePressed, this, &ofApp::sendCall);
            success = true;
            cout<<"\n\n Successful call"<<success<<"\n\n";
            uiLock.lock();
            callButtonUI->setVisible(false);
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
    if(!success){
        //TODO fix this, it shows up when you successfully call
        ofSystemAlertDialog("Your call was not successfully sent. Make sure you selected a person and that they are available to call");
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
        xmppCaller->setVisible(false);
        logoutUI->setVisible(false);
        uiLock.unlock();
        
        
        callingState = InCall;
        state = IN_CALL;
	}
}

// will be called whenever the call ends and
// receives the reason as parameter
void ofApp::onCallFinished(ofxXMPPTerminateReason & reason){
    
	if(callingState==Calling){
		// if we started a call most likely the other end declined it
		// or the call failed
		ofSystemAlertDialog("Call declined");
        
        uiLock.lock();
        delete callNotification;
        
        callButtonUI->setVisible(true);
        ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
        
        uiLock.unlock();
        
        //reset states
        callingState = Disconnected;
        calling = -1;
        // reset the rtp element to be able to start a new call
        rtp.setup(50);
        rtp.setStunServer("132.177.123.6");
        rtp.addSendVideoChannel(640,480,30);
        rtp.addSendAudioChannel();
        
        
    }
    else{
        //reset states
        callingState = Disconnected;
        calling = -1;
        
        // reset the rtp element to be able to start a new call
        rtp.setup(50);
        rtp.setStunServer("132.177.123.6");
        rtp.addSendVideoChannel(640,480,30);
        rtp.addSendAudioChannel();
        
        uiLock.lock();
        endCallUI->setVisible(false);
        ofAddListener(endCallButton->mousePressed, this, &ofApp::endCall);
        if(xmppCaller){
            xmppCaller->setVisible(true);
        }
        else
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
    if(state==LOGIN_SCREEN){
        if(key == OF_KEY_TAB){
            loginGUI->switchInputs();
        }
        if(key==OF_KEY_RETURN){
            bool temp = true;
            loginGUI->trigger(temp);
        }
    }
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

