/*
 * draggableRect.h
 *
 * an ofxUIRectangle that can be dragged around with the mouse
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#ifndef DRAGGABLE_RECT
#define DRAGGABLE_RECT

#include "ofxUIRectangle.h"
#include "ofMain.h"

class draggableRect : public ofxUIRectangle {

public:

    ~draggableRect();
    draggableRect(float x, float y, float w, float h, ofRectangle boundsRect);
    bool isHit();
    float getCenterX();
    float getCenterY();
    ofxUIRectangle * getBounds();
    void setHeight(float h);
    void setImage(string imagePath);
    void removeImage();
    void draw();
    virtual void onMousePressed(ofMouseEventArgs &e);
    virtual void onMouseReleased(ofMouseEventArgs &e);
    virtual void onMouseDragged(ofMouseEventArgs &e);

protected:

    bool hit;
    float mouseDx, mouseDy;
    ofxUIRectangle * bounds;
    ofImage * image;

};

#endif
