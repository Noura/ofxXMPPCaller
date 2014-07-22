/*
 * multiLineTextInput.cpp
 *
 * text input that wraps around to multiple lines
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#include "multiLineTextInput.h"

multiLineTextInput::multiLineTextInput(string _name, string _textString, float w, float h, float x, float y, int _size)
: ofxUITextArea(string(_name),  string(_textString), w, h, x, y, _size)
, clicked(false)
, autoclear(true)
, triggerOnClick(true)
, triggerType(OFX_UI_TEXTINPUT_ON_FOCUS)
, cursorOffset(2.0)
, theta(0)
, cursorChar(0)
, cursorLine(0)
, shiftKeyPressed(false)
, xCorrection(0) // TODO fix it so I don't need this hack
, yCorrection(0)
, cursorWidth(2.0)
{
    name = string(_name);
    defaultstring = string(_textString);
    //kind = OFX_UI_WIDGET_MULTILINETEXTINPUT;
    
    // TODO this is all a bad hack
    float margin_guess = 2; // TODO fix it so I don't need this hack
    paddedRect = new ofxUIRectangle(x + margin_guess, y + margin_guess, w - 2.0 * margin_guess, h - 2.0 * margin_guess);
    rect = new ofxUIRectangle(x + padding + margin_guess, y + padding + margin_guess, w - 2.0 * padding - 2.0 * margin_guess, h - 2.0 * padding - 2.0 * margin_guess);
    paddedRect->setParent(rect);

    float heightLimit = rect->getHeight();
    
    if (autoSize) {
        numLinesLimit = 0;
    } else {
        float denominator = lineHeight + lineSpaceSize;
        if (denominator == 0) { // before fonts are initialized
            denominator = OFX_UI_FONT_LARGE_SIZE;
        }
        numLinesLimit = (int)heightLimit / denominator;
    }
    
    //textArea = new ofxUITextArea(name, defaultstring, rect->width, rect->height, rect->x, rect->y, _size);
    //addEmbeddedWidget(textArea);
}

// TODO let the user set the cursor width with a float or to be the same width as a character in the textArea's font

void multiLineTextInput::moveCursorBackward() {
    if(cursorChar > 0) {
        cursorChar--;
    } else if (cursorLine > 0) {
        cursorLine--;
        cursorChar = textLines[cursorLine].size() - 1;
    }
    cursorLine = CLAMP(cursorLine, 0, textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textLines[cursorLine].size());
}

void multiLineTextInput::moveCursorForward() {
    if (textstring == "") {
        cursorChar = 0;
        cursorLine = 0;
        return;
    }
    if(cursorChar < textLines[cursorLine].size()) {
        cursorChar++;
    } else if (cursorLine < textLines.size() - 1) {
        cursorLine++;
        cursorChar = 0;
    }
    cursorLine = CLAMP(cursorLine, 0, textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textLines[cursorLine].size());
}


int multiLineTextInput::getStringIndex() {
    // returns the index in textArea->textstring corresponding with the
    // cursor position specified by cursorChar and cursorLine
    // cursorChar x is between textArea->textstring[x-1] and textArea->textstring[x]
    // cursorChar = 0 is before textArea->textstring[0]
    int i = 0;
    int currentLineIndex = 0;
    while (currentLineIndex < cursorLine) {
        i += textLines[currentLineIndex++].size();
    }
    i += cursorChar;
    i = CLAMP(i, 0, textstring.size());
    return i;
}

int multiLineTextInput::setCursorPosition(int stringIndex) {
    // sets cursorChar and cursorLine to correspond with the stringIndex
    if (textLines.size() == 0) {
        cursorLine = 0;
        cursorChar = 0;
        return;
    }
    
    int count = 0; // count up to stringIndex
    int i = 0;
    
    for (int i = 0; i < textLines.size(); i++) {
        if (count + textLines[i].size() >= stringIndex) {
            cursorLine = i;
            cursorChar = stringIndex - count;
            break;
        } else {
            count += textLines[i].size();
        }
    }
    cursorLine = CLAMP(cursorLine, 0, textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textLines[cursorLine].size());
}

void multiLineTextInput::clearText() {
    textstring.clear();
    formatTextString();
    cursorChar = 0;
    cursorLine = 0;
}

void multiLineTextInput::drawCursor() {
    ofxUILabel * label = getLabelWidget();

    string beforeCursor = "";
    if (textLines.size() > 0) {
        beforeCursor = textLines[cursorLine].substr(0, cursorChar);
    }

    // we need to put "." on both ends of the string so that the width of
    // white space at the beginning or end of the string is included
    float xOffset = label->getStringWidth("." + beforeCursor + ".") - label->getStringWidth("..");
    
    float x = getRect()->getX() + xOffset;
    float y = getLineTopY(cursorLine);
    
    ofxUIRectangle * parentRect = parent->getRect();
    x = CLAMP(x, parentRect->x + rect->x, parentRect->x + rect->x + rect->width - padding);
    y = CLAMP(y, parentRect->y + rect->y, parentRect->y + rect->y + rect->height - padding);

    
    // cursor color oscillates
    ofxUIFill();
    ofxUISetColor(getLabelWidget()->getColorFillHighlight(), 255.0*fabs(cos(theta)));
    theta +=0.05;
    ofxUIDrawRect(x + xCorrection, y + yCorrection, cursorWidth, lineHeight);
}

void multiLineTextInput::drawFill() {
    if(draw_fill)
    {
        ofxUIFill(); 
        ofxUISetColor(color_fill); 
        rect->draw();
        
        if(drawShadow)
        {
            ofSetColor(color_back);
            if(textLines.size()>numLinesLimit){
                
                for(unsigned int i = 0; i < numLinesLimit; i++)
                {
                    label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[textLines.size()-numLinesLimit+i]);
                }
            }else{
                
                for(unsigned int i = 0; i < textLines.size(); i++)
                {
                    label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
                }
            }
        }
        
        ofSetColor(color_fill);
        if(textLines.size()>numLinesLimit){
            
            for(unsigned int i = 0; i < numLinesLimit; i++)
            {
                label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[textLines.size()-numLinesLimit+i]);
            }
        }else{
            
            for(unsigned int i = 0; i < textLines.size(); i++)
            {
                label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
            }
        }
    }
    
    //drawFill();
    if(clicked)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        rect->draw();
        drawCursor();
    }
    if(textstring == "" && !clicked)
    {
        ofxUIFill(); 
        ofxUISetColor(color_fill);
        getLabelWidget()->drawString(rect->x + 2.0, yCorrection + getLineBottomY(0), defaultstring);
    }
}

// TODO maybe I should inherit from ofxUITextInput so I don't have to re-implement mouseMoved etc

void multiLineTextInput::mouseMoved(int x, int y) {
    if(rect->inside(x, y))
    {
        state = OFX_UI_STATE_OVER;         			
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();         
}

void multiLineTextInput::mouseDragged(int x, int y, int button) {
    if(hit)
    {
        state = OFX_UI_STATE_DOWN;         
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;  
    }
    stateChange();     
}

void multiLineTextInput::mousePressed(int x, int y, int button) {
    if(rect->inside(x, y))
        focus();
    else
        unfocus();
}

void multiLineTextInput::mouseReleased(int x, int y, int button) {
    if(hit)
    {
#ifdef TARGET_OPENGLES
        state = OFX_UI_STATE_NORMAL;        
#else            
        state = OFX_UI_STATE_OVER; 
#endif 
//			triggerType = OFX_UI_TEXTINPUT_ON_UNFOCUS; 
//			triggerEvent(this); 
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;         
    }
    hit = false;
    stateChange();         
}

void multiLineTextInput::keyPressed(int key) {
    // TODO might be cleaner with an if (!clicked) return; instead
    if(clicked)
    {
        switch (key)
        {
            case OF_KEY_BACKSPACE:
            {
                if (textstring.size() > 0) {
                    int i = getStringIndex();
                    if (i > 0) {
                        textstring.erase(i - 1, 1);
                        formatTextString();
                        setCursorPosition(i - 1);
                    }
                }
            }
                break;

            case OF_KEY_DEL:
            {
                if (textstring.size() > 0) {
                    int i = getStringIndex();
                    if (i < textstring.size()) {
                        textstring.erase(i, 1);
                        formatTextString();
                        setCursorPosition(i);
                    }
                }
            }
                break;
                
            case OF_KEY_RETURN:
                triggerType = OFX_UI_TEXTINPUT_ON_ENTER;
                triggerEvent(this);
                if(autoclear) {
                    string input = textstring.substr();
                    clearText();
                    clicked = true;
                }
                break;
                
            case OF_KEY_RIGHT:
                moveCursorForward();
                break;					

            case OF_KEY_LEFT:
                moveCursorBackward();
                break;
                
            case OF_KEY_DOWN:
                if (cursorLine < textLines.size() - 1) {
                    cursorLine++;
                    cursorChar = CLAMP(cursorChar, 0, textLines[cursorLine].size());
                }
                break;
                
            case OF_KEY_UP:
                if (cursorLine > 0) {
                    cursorLine--;
                    cursorChar = CLAMP(cursorChar, 0, textLines[cursorLine].size());
                }
                break;

#if (OF_VERSION_MINOR > 7)
            case OF_KEY_TAB:
            case OF_KEY_COMMAND:
            case OF_KEY_CONTROL:
            case OF_KEY_LEFT_CONTROL:
            case OF_KEY_RIGHT_CONTROL:
            case OF_KEY_LEFT_ALT:
            case OF_KEY_RIGHT_ALT:
            case OF_KEY_LEFT_SUPER:
            case OF_KEY_RIGHT_SUPER:					
#endif
            case OF_KEY_F1:
            case OF_KEY_F2:
            case OF_KEY_F3:
            case OF_KEY_F4:
            case OF_KEY_F5:
            case OF_KEY_F6:
            case OF_KEY_F7:
            case OF_KEY_F8:
            case OF_KEY_F9:
            case OF_KEY_F10:
            case OF_KEY_F11:
            case OF_KEY_F12:
            case OF_KEY_PAGE_UP:
            case OF_KEY_PAGE_DOWN:
            case OF_KEY_HOME:
            case OF_KEY_END:
            case OF_KEY_INSERT:
            case OF_KEY_ALT:
            break;
              
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
            case OF_KEY_SHIFT:
                shiftKeyPressed = true;
                break;
                
            default:
            {   
                char c = key;
                /*
                if (key == OFXUI_KEY_BACK_QUOTE) { //TODO better way to do this
                    c = '`';
                }
                if (shiftKeyPressed) {
                    if (key == OFXUI_KEY_BACK_QUOTE) c = '~';
                    if (key == '1') c = '!';
                    if (key == '2') c = '@';
                    if (key == '3') c = '#';
                    if (key == '4') c = '$';
                    if (key == '5') c = '%';
                    if (key == '6') c = '^';
                    if (key == '7') c = '&';
                    if (key == '8') c = '*';
                    if (key == '9') c = '(';
                    if (key == '0') c = ')';
                    if (key == '-') c = '_';
                    if (key == '=') c = '+';
                    if (key == '[') c = '{';
                    if (key == ']') c = '}';
                    if (key == '\\') c = '|';
                    if (key == ';') c = ':';
                    if (key == '\'') c = '"';
                    if (key == ',') c = '<';
                    if (key == '.') c = '>';
                    if (key == '/') c = '?';
                }
                 */
                int i = getStringIndex();
                textstring.insert(i, 1, c);
                formatTextString();
                setCursorPosition(i + 1);
            }
                break;
        }
        //cout<<textLines.size()<<"\n";
        //cout<<textstring<<"\n\n";
    }
}

/*
void multiLineTextInput::keyReleased(int key) {
    // TODO if (!clicked)...
    if (clicked) {
        switch (key) {
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
            case OF_KEY_SHIFT:
                shiftKeyPressed = false;
                break;
            default:
                break;
        }
    }
}
*/



void multiLineTextInput::unClick() {
    if(clicked)
    {
        clicked = false;          
        triggerType = OFX_UI_TEXTINPUT_ON_UNFOCUS; 
        triggerEvent(this);             
    }
}

void multiLineTextInput::focus() {
    clicked = true;
    theta = 0;
    hit = true;
    
    setCursorPosition(textstring.size());
    
    state = OFX_UI_STATE_DOWN;
    triggerType = OFX_UI_TEXTINPUT_ON_FOCUS;
    
    if(triggerOnClick) triggerEvent(this);
    
    stateChange();
}

void multiLineTextInput::unfocus() {
    state = OFX_UI_STATE_NORMAL;
    if(clicked) unClick();
    stateChange();
}

void multiLineTextInput::stateChange() {
    // TODO maybe instead of having to call stateChange() every time we change state = , this method should assign to state and do these things in reaction
    switch (state) {
        case OFX_UI_STATE_NORMAL:
        {            
            draw_fill_highlight = false;             
            draw_outline_highlight = false;  
            getLabelWidget()->unfocus();
        }
            break;
        case OFX_UI_STATE_OVER:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = true;  
            getLabelWidget()->focus();
        }
            break;
        case OFX_UI_STATE_DOWN:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = true;             
            getLabelWidget()->focus();
        }
            break;
        case OFX_UI_STATE_SUSTAINED:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = false;                         
            getLabelWidget()->unfocus();
        }
            break;                            
        default:
            break;
    }        
}

string multiLineTextInput::getTextString() {
    return textstring;
}

void multiLineTextInput::setTextString(string s) {
    setTextString(s);
    cursorChar = 0;
}

void multiLineTextInput::setVisible(bool _visible) {
    visible = _visible;
    setVisible(visible);
}

bool multiLineTextInput::isClicked() {
    return clicked; 
}
/*
ofxUITextArea* multiLineTextInput::getTextArea() {
    return textArea;
}
*/
void multiLineTextInput::setTriggerType(int _triggerType) {
    triggerType = _triggerType;
}

int multiLineTextInput::getTriggerType() {
    return triggerType;
}

//TODO make sure trigger types work. also what is setTriggerOnClick for? how is that different than the other triggers?
void multiLineTextInput::setAutoClear(bool _autoclear) {
    autoclear = _autoclear;
}

void multiLineTextInput::setTriggerOnClick(bool _triggerOnClick) {
    triggerOnClick = _triggerOnClick;
}

float multiLineTextInput::getLineBottomY(int line_index) {
    return rect->getY() + (line_index + 1) * (lineHeight + lineSpaceSize) - lineSpaceSize;
}

float multiLineTextInput::getLineTopY(int line_index) {
    return getLineBottomY(line_index) - lineHeight;
}

//Does this still supersede the ofxUITextArea method if it is not declared virtual there??

void multiLineTextInput::formatTextString()
{
    /* ofxUI's version
    float rectWidthLimit = rect->getWidth()-padding*6;
    float rectHeightLimit = rect->getHeight()-label->getLineHeight()-padding;
    bool overheight = false;
    
    lineHeight = label->getStringHeight("1");
    lineSpaceSize = padding*2;
    
    offsetY = floor(padding*.125);
    if(label->getStringWidth(textstring) <= rectWidthLimit)
    {
        cout<<rectWidthLimit<<" "<<label->getStringWidth(textstring)<<" not\n";
        if(textstring.size() > 0)
        {
            textLines.push_back(textstring);
        }
    }
    else
    {
        cout<<rectWidthLimit<<" "<<label->getStringWidth(textstring)<<" yes\n";
        float tempWidth;
        float tempHeight;
        textLines.clear();
        string line = "";
        size_t i=0;
        
        while (i < textstring.size() && !overheight) //if not at the end of the string && not over the rect's height
        {
            tempWidth = label->getStringWidth(line);
            if(tempWidth < rectWidthLimit)
            {
                line+=textstring.at(i);
                i++;
                if(i == textstring.size())
                {
                    textLines.push_back(line);
                }
            }
            else
            {
                bool notFound = true;
                
                while (notFound && !overheight)
                {
                    if(strncmp(&textstring.at(i), " ",1) == 0)
                    {
                        tempHeight = (textLines.size()+1)*(lineHeight+lineSpaceSize);
                        //                        cout << tempHeight << endl;
                        //                        cout << rectHeightLimit << endl;
                        if(!autoSize && tempHeight >= rectHeightLimit)
                        {
                            textLines.push_back(line);
                            textLines[textLines.size()-1]+="...";
                            overheight = true;
                        }
                        notFound = false;
                        if(!overheight)
                        {
                            textLines.push_back(line);
                            line.clear();
                            i++;
                        }
                    }
                    else
                    {
                        i--;
                        line.erase(line.end()-1);
                    }
                }
            }
        }
    }
    
    if(autoSize)
    {
        rect->setHeight((lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize);
    }
    
    if(overheight)
    {
        rect->setHeight(MAX(rect->getHeight(),(lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize));
    }
     */
    
    //ofxUI fork version
    textLines.clear();
    
    lineHeight = label->getStringHeight("1");
    lineSpaceSize = label->getStringWidth(" ");
    
    float widthLimit = rect->getWidth() - label->getStringWidth("M")- 2 * padding;
    if (widthLimit < label->getStringWidth("M")) {
        return;
    }
    float heightLimit = rect->getHeight();
    
    if (!autoSize){
        float denominator = lineHeight + lineSpaceSize;
        if (denominator == 0) { // before fonts are initialized
            denominator = OFX_UI_FONT_LARGE_SIZE;
        }
        numLinesLimit = (int)heightLimit / denominator;
    }
    
    string line = "";
    for (int i = 0; i < textstring.size(); i++) {
        if (!autoSize && textLines.size() > numLinesLimit) {
            break;
        }
        if (label->getStringWidth(line) >= widthLimit) {
            if (line.size() == 0) {
                break;
            }
            // try to break line at white space
            int whitespace_index = line.size() - 1;
            while (whitespace_index > 0 &&
                   !isspace(line.at(whitespace_index))) {
                whitespace_index--;
            }
            if (whitespace_index <= 0) {
                // white space not found, or found at first char
                textLines.push_back(line.substr(0, line.size()-1));
                line = line.substr(line.size()-1);
            } else {
                // white space found
                textLines.push_back(line.substr(0, whitespace_index));
                line = line.substr(whitespace_index);
            }
        }
        char c = textstring.at(i);
        line += c;
    }
    
    
    if (line != "" ){ //&& !(!autoSize && textLines.size() > numLinesLimit)) {
        textLines.push_back(line);
    }
    
    if(autoSize) {
        rect->setHeight((lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize);
    }
    
    //cout<<cursorLine<<" "<<numLinesLimit<<"\n";
}