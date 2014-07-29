//
//  passwordInput.cpp
//  example
//
//  Created by Catherine Liu on 7/24/14.
//
//

#include "passwordInput.h"

passwordInput::passwordInput(string _name, string _textstring, float w, float h, float x, float y, int _size):
ofxUITextInput(_name, _textstring, w, h, x, y ,_size){
    
}
void passwordInput::recalculateDisplayString()
{
    // the maximum width of the displaystring
    float maxWidth = rect->getWidth()-padding*2.0;
    
    displaystring = string(textstring.size(), '*');//+textstring.substr(textstring.size()-1);
    //displaystring = textstring;
    string stringBeforeCursor = displaystring.substr(0, cursorPosition);
    string stringBeforeLabel =  displaystring.substr(0, firstVisibleCharacterIndex);
    
    // if the cursoroffset - length of the (invisible) string before the label < 0, we have to shift our string to the left to get our cursor in the label
    while(label->getStringWidth(stringBeforeCursor) - label->getStringWidth(stringBeforeLabel) < 0){
        firstVisibleCharacterIndex --;
        stringBeforeLabel =  displaystring.substr(0, firstVisibleCharacterIndex);
    }
    
    // if the cursoroffset - length of the (invisible) string before the label is > maximum width, we have to shift to the right
    while(label->getStringWidth(stringBeforeCursor) - label->getStringWidth(stringBeforeLabel) > maxWidth){
        firstVisibleCharacterIndex ++;
        stringBeforeLabel =  displaystring.substr(0, firstVisibleCharacterIndex);
    }
    
    // we now know how long the string before the label should be, so trim it off
    displaystring = displaystring.substr(MIN(firstVisibleCharacterIndex, displaystring.length()));
    
    // trim off the end of the string until it fits
    while(label->getStringWidth(displaystring) > maxWidth && displaystring.length() > 0)
    {
        displaystring = displaystring.substr(0, displaystring.size()-1);
    }
    
    label->setLabel(displaystring);
}

void passwordInput::drawFill()
{
    if(draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        rect->draw();
    }
    if(clicked)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        rect->draw();
        
        float h = label->getRect()->getHeight();
        
        float ph = rect->getHeight();
        label->getRect()->setY(ph/2.0 - h/2.0);
        
        ofxUIFill();
        ofxUISetColor(label->getColorFillHighlight(), 255.0*fabs(cos(theta)));
        theta +=0.05;
        recalculateDisplayString();
        int displayCursorPosition = cursorPosition - firstVisibleCharacterIndex;
        string displayStringBeforeCursor = displaystring.substr(0, displayCursorPosition);
        spaceOffset = label->getStringWidth(displayStringBeforeCursor);
        
        float x = label->getRect()->getX()+spaceOffset;
        float y = label->getRect()->getY()-padding;
        float t = label->getRect()->getHeight()+padding*2.0;
        ofxUIDrawRect(x, y, cursorWidth, t);
    }
    
    if(textstring.size() == 0 && !clicked)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        label->drawString(rect->getX()+defaultX, rect->getY()+defaultY, defaultstring);
    }
}
