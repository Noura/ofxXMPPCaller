/*
 * YesNoDialog.cpp
 *
 *
 *  Created on: Oct 7, 2013
 *      Author: noura
 */

#include "YesNoDialog.h"

YesNoDialog::YesNoDialog(float _x, float _y, float _w, float _h, std::string _msg)
: x(_x), y(_y), w(_w), h(_h)
, msg(_msg)
, canvas(NULL)
, prompt(NULL)
, yesButton(NULL)
, noButton(NULL) {
}

YesNoDialog::~YesNoDialog() {
    ofRemoveListener(yesButton->mousePressed, this, &YesNoDialog::Yes);
    ofRemoveListener(noButton->mousePressed, this, &YesNoDialog::No);
    delete canvas;
}

void YesNoDialog::setup() {
    float margin = 20;
    float buttonWidth = (w - 3.0 * margin) / 2.0;
    float buttonHeight = 20;
    
    ofxUIColor black(0, 0, 0);
    ofxUIColor white(255, 255, 255);
    ofxUIColor gray(100, 100, 100);
    
    canvas = new ofxUICanvas(x, y, w, h);
    canvas->setFont("GUI/NewMediaFett.ttf");
    canvas->setColorBack(black);
    
    prompt = new ofxUITextArea(msg, msg, w - 2.0 * margin, h - buttonHeight - 3.0 * margin, margin, margin, OFX_UI_FONT_SMALL_SIZE);
    prompt->getLabelWidget()->setFont(canvas->getFontSmall());
    prompt->setColorBack(white);
    prompt->setColorFill(black);
    
    noButton = new CustomEventLabelButton("No", buttonWidth, buttonHeight, margin, h - margin - buttonHeight, OFX_UI_FONT_SMALL_SIZE);
    noButton->getLabelWidget()->setFont(canvas->getFontSmall());
    noButton->setColorBack(gray);
    noButton->setColorFill(white);
    noButton->setColorOutline(white);
    
    yesButton = new CustomEventLabelButton("Yes", buttonWidth, buttonHeight, w - margin - buttonWidth, h - margin-buttonHeight, OFX_UI_FONT_SMALL_SIZE);
    yesButton->getLabelWidget()->setFont(canvas->getFontSmall());
    yesButton->setColorBack(gray);
    yesButton->setColorFill(white);
    yesButton->setColorOutline(white);
    //TODO Fix this
    //error on adding prompt with lineHeight->formatTextString->setParent for textArea->addWidget of canvas?
    canvas->addWidget(prompt);
    canvas->addWidget(noButton);
    canvas->addWidget(yesButton);

    ofAddListener(yesButton->mousePressed, this, &YesNoDialog::Yes);
    ofAddListener(noButton->mousePressed, this, &YesNoDialog::No);
}

void YesNoDialog::draw() {
    canvas->draw();
}

void YesNoDialog::Yes(bool & val) {
    bool response = true;
    ofNotifyEvent(answer, response, this);
}

void YesNoDialog::No(bool & val) {
    bool response = false;
    ofNotifyEvent(answer, response, this);
}
