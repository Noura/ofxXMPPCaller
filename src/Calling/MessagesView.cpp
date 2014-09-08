/*
 *  MessagesView.cpp
 *
 *  Manages the friend-to-friend chat messages from RTP
 *  Messages is the model, MessagesView is the view
 *
 *  Created on: Sep 9, 2013
 *      Author: noura
 */

#include "MessagesView.h"
#include "Messages.h"

MessagesView::MessagesView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, title_h(30.0)
, appState(_appState)
, xmpp(_xmpp)
, messagesCanvas(NULL)
, composingCanvas(NULL)
, composingMsg(NULL)
, sharedResources(NULL)
, visible(true)
, chatLabel(NULL)
{
    canvas_h = h * CONVERSATION_PERCENT_HEIGHT/100.0 - title_h;
}
MessagesView::MessagesView(float _x, float _y, float _w, float _h, SharedStateBundle * _appState, shared_ptr<ofxXMPP> _xmpp, ofxUICanvas*_sharedResources)
: x(_x)
, y(_y)
, w(_w)
, h(_h)
, title_h(30.0)
, appState(_appState)
, xmpp(_xmpp)
, sharedResources(_sharedResources)
, messagesCanvas(NULL)
, composingCanvas(NULL)
, composingMsg(NULL)
, visible(true)
, chatLabel(NULL)
{
    canvas_h = h * CONVERSATION_PERCENT_HEIGHT/100.0 - title_h;
}

MessagesView::~MessagesView() {
    if (model)
        ofRemoveListener(model->newMessage, this, &MessagesView::addMessage);
    
    if (composingMsg)
        ofRemoveListener(composingMsg->inputSubmitted, this, &MessagesView::onNewLocalMessage);
    
    if(messagesCanvas)
        delete messagesCanvas;
    if(composingCanvas)
        delete composingCanvas;
    if(chatLabel)
        delete chatLabel;
}

void MessagesView::setModel(Messages * _model) {
    model = _model;
    ofAddListener(model->newMessage, this, &MessagesView::addMessage);
}

void MessagesView::setup() {
    ofxXMPPUser user = appState->chatContact;
    
    title = FriendView::formatUserName(user.userName);
    
    chatLabel = new ofxUICanvas(x, y, w, title_h);
    chatLabel->setDrawOutline(true);
    chatLabel->setColorBack(ofColor(255));
    chatLabel->setColorFill(ofColor(0));
    chatLabel->addLabel("Chat with "+title);
    
    if(sharedResources)
        messagesCanvas = new dynamicListVerticalScrollbarCanvas(x, y + title_h, w, canvas_h, sharedResources);
    else{
        messagesCanvas = new dynamicListVerticalScrollbarCanvas(x, y + title_h, w, canvas_h);
        messagesCanvas->setFont("GUI/NewMediaFett.ttf");
    }
    messagesCanvas->getScrollbar()->setImage("GUI/scrollbar.png");
    for (int i = 0; i < model->messages.size(); i++ ) {
        addMessage(model->messages[i]);
    }
    
    composingCanvas = new ofxUICanvas(x, y + title_h + canvas_h, w, h - title_h - canvas_h, sharedResources);
    float margin = OFX_UI_GLOBAL_PADDING + OFX_UI_GLOBAL_WIDGET_SPACING;
    composingMsg = new multiLineTextInput("composing", "", w - 2.0 * margin + 4.0, h - title_h - canvas_h - 2.0 * margin + 4.0, margin, margin);
    composingCanvas->addWidgetDown(composingMsg);
    
    //set the cursor color
    composingMsg->getLabelWidget()->setColorFillHighlight(ofColor(0));
    composingMsg->getLabelWidget()->setColorBack(ofColor(0));
    composingCanvas->centerWidgets();
    /*TODO
     add in a setFocus to multiline input later on
     composingMsg->setFocus(true);
     */
    
    ofAddListener(composingMsg->inputSubmitted, this, &MessagesView::onNewLocalMessage);
    
}

void MessagesView::onNewLocalMessage(string &msg) {
    ofNotifyEvent(newLocalMessage, msg, this);
}

void MessagesView::addMessage(ofxXMPPMessage &msg) {
    string text = formatMessage(msg);
    //input height = 0 for autosizing height
    ofxUITextArea * messageView = new ofxUITextArea(text, text, w -  dynamicListVerticalScrollbarCanvas::scrollbarDefaultWidth - 2.0, 0, 5, 5);
    messageView->getLabelWidget()->setVisible(false);
    messagesCanvas->addWidget(messageView);
    messagesCanvas->scrollToBottom();
}

string MessagesView::formatMessage(ofxXMPPMessage msg) {
    string from = msg.from.substr();
    int i = from.find('@');
    if (i < from.size()) {
        from = from.substr(0, i);
    }
    return from + ": " + msg.body;
}

void MessagesView::setVisible(bool _visible){
    visible = _visible;
    if(messagesCanvas)
        messagesCanvas->setVisible(visible);
    if(composingCanvas)
        composingCanvas->setVisible(visible);
    if(chatLabel)
        chatLabel->setVisible(visible);
    
}

void MessagesView::draw() {
    if(visible){
        chatLabel->draw();
        messagesCanvas->draw();
        composingCanvas->draw();
    }
}

void MessagesView::update() {
    if(messagesCanvas)
        messagesCanvas->update();
}