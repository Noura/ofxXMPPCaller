#include "ofApp.h"
#include "ofxNice.h"
#include "ofxGStreamer.h"
//#define BUNDLED

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef BUNDLED
    ofxGStreamerSetBundleEnvironment();
#endif
    /*
     ofXml settings;
     settings.load("settings.xml");
     string server = settings.getValue("server");
     string user = settings.getValue("user");
     string pwd = settings.getValue("pwd");
     */
    
    server = "talk.google.com";
    
    grabber.initGrabber(640,480);
    
    setupLoginScreen();
    
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
    
    ofColor dark(100, 100, 100);
    logoutUI->setColorBack(dark);
}
void ofApp::logout(bool &e){
    
    delete xmppCaller;
    delete logoutUI;
    
	ofRemoveListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofRemoveListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofRemoveListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
    
    setupLoginScreen();
    //rtp.close();
    //TODO figure out how to stop the xmppRTP connection?X
    //rtp.getXMPP().stop();
    //rtp.getClient().close();
}

void ofApp::setupCallButton(){
    float unlaunchW = 75;
    float unlaunchH = 40;
    float margin = 3;
    float unlaunchX = grabberX + grabberWidth/2-unlaunchW/2;
    float unlaunchY = 250+15+grabberHeight+15;
    
    // unlaunchCanvas and unlaunchButton will unlaunch or "close" the chat UI
    callButtonUI = new ofxUICanvas(unlaunchX, unlaunchY, unlaunchW, unlaunchH);
    callButtonUI->setFont("GUI/NewMediaFett.ttf");
    callButton = new CustomEventLabelButton("Start Call", unlaunchW - 2.0 * margin, unlaunchH - 2.0 * margin, 0, 0, OFX_UI_FONT_SMALL);
    callButtonUI->addWidget(callButton);
    
    ofAddListener(callButton->mousePressed, this, &ofApp::sendCall);
    
    callButtonUI->setColorBack(ofColor(150));
}

void ofApp::sendCall(bool &e){
 cout<<"\n\n"<<"CALLING SOMEPERSON"<<"\n\n";
}

void ofApp::onCallReceived(string & from){
	rtp.acceptCall();
    state = IN_CALL;
    //TODO move the grabber's X/Y to bottom left of the remote video
    //Get rid of xmppCaller contacts view
    //if possible keep the chat and move to right side
}

void ofApp::onCallAccepted(string & from){
    
    state = IN_CALL;
    
    //TODO move the grabber's X/Y to bottom left of the remote video
    //Get rid of xmppCaller contacts view
    //if possible keep chat and move to right side
}

void ofApp::onCallFinished(ofxXMPPTerminateReason & reason){
    //restart callManager
    state = CALL_MANAGER;
}


void ofApp::exit(){
}


//--------------------------------------------------------------
void ofApp::update(){
    if(state == LOGIN_SCREEN)
        loginGUI->update();
    if(state==IN_CALL){
        //TODO update remote video, audio????
    }
    
    if(state==CALL_MANAGER){
        xmppCaller->update();
    }
    
	grabber.update();
    //TODO, your audio???
    
    
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

//--------------------------------------------------------------
void ofApp::draw(){
    if(state==LOGIN_SCREEN)
        loginGUI->draw();
    
	grabber.draw(grabberX,grabberY,grabberWidth,grabberHeight);
    if(state == CALL_MANAGER)
        xmppCaller->draw();
    /*
     ofSetColor(255);
     remoteVideo.draw(0,0);
     grabber.draw(400,300,240,180);
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /*
     if(key==OF_KEY_UP){
     guiState++;
     guiState%=2;
     }else if(key==OF_KEY_DOWN){
     guiState--;
     guiState%=2;
     }*/
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
        
        /*xml settings
         settings->setValue("settings:user", user+"@gmail.com");
         settings->setValue("settings:pwd", password);
         settings->saveFile("settings.xml");
         */
        return true;
    }
    
}

void ofApp::setupRTP(){
    
    state = CALL_MANAGER;
    delete loginGUI;
    
    //grabber.initGrabber(640,480);
	//remoteVideo.allocate(640,480,GL_RGB);
    
	//ofxNiceEnableDebug();
    
    //move rtp setup into the login thing?
    
	rtp.setup(500);
	rtp.setStunServer("132.177.123.6");
	rtp.getXMPP().setCapabilities("telekinect");
    
	rtp.connectXMPP(server,user,pass);
    
    //TODO check that the connection is properly done, otherwise they entered the wrong info for user/pass
    //and wont be able to see any contacts
	rtp.addSendVideoChannel(640,480,30);
	rtp.addSendAudioChannel();
	calling = -1;
    
	ofBackground(255);
    
	ofAddListener(rtp.callReceived,this,&ofApp::onCallReceived);
	ofAddListener(rtp.callFinished,this,&ofApp::onCallFinished);
	ofAddListener(rtp.callAccepted,this,&ofApp::onCallAccepted);
   
    ofBackground(ofColor(OFX_UI_COLOR_BACK_ALPHA));
    grabberX = 1000-grabberWidth-50;
    grabberY = 250+15;
    
    xmppCaller = new ofxXMPPCaller(0,0,server,user, pass,"Login", "telekinect");
    xmppCaller->setup();
    
    //todo(figure out how to do this only if the person can telekinect)
    setupCallButton();
    //setupLogout();
    
}
