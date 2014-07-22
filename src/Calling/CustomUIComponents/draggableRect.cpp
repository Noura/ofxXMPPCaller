/*
 * draggableRect.h
 *
 * an ofxUIRectangle that can be dragged around with the mouse
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#include "draggableRect.h"

draggableRect::~draggableRect() {
    delete bounds;
    delete image;
    ofRemoveListener(ofEvents().mousePressed, this, &draggableRect::onMousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &draggableRect::onMouseReleased);
    ofRemoveListener(ofEvents().mouseDragged, this, &draggableRect::onMouseDragged);
}

draggableRect::draggableRect(float x, float y, float w, float h, ofRectangle boundsRect)
: ofxUIRectangle(x, y, w, h)
, hit(false)
, mouseDx(0)
, mouseDy(0)
, image(NULL) {
    bounds = new ofxUIRectangle(boundsRect.x, boundsRect.y, boundsRect.width, boundsRect.height);
    ofAddListener(ofEvents().mousePressed, this, &draggableRect::onMousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &draggableRect::onMouseReleased);
    ofAddListener(ofEvents().mouseDragged, this, &draggableRect::onMouseDragged);
}

bool draggableRect::isHit() {
    return hit;
}

float draggableRect::getCenterX() {
    return getMinX() + width/2;
}

float draggableRect::getCenterY() {
    return getMinY() + height/2;
}

ofxUIRectangle* draggableRect::getBounds() {
    return bounds;
}

void draggableRect::setHeight(float h) {
    ofxUIRectangle::setHeight(h);
    if (image) {
        image->resize(width, height);
    }
}

void draggableRect::setImage(string imagePath) {
    delete image;
    image = new ofImage();
    image->loadImage(imagePath);
    image->resize(width, height);
}

void draggableRect::removeImage() {
    delete image;
    image = NULL;
}

void draggableRect::draw() {
    ofxUIRectangle::draw();
    if (image) {
        ofPushStyle();
        ofSetColor(255);
        image->draw(x, y);
        ofPopStyle();
    }
}

void draggableRect::onMousePressed(ofMouseEventArgs &e) {
    if (inside(e.x, e.y)) {
        hit = true;
        mouseDx = e.x - x;
        mouseDy = e.y - y;
    }
}

void draggableRect::onMouseReleased(ofMouseEventArgs &e) {
    hit = false;
    mouseDx = 0;
    mouseDy = 0;
}

void draggableRect::onMouseDragged(ofMouseEventArgs &e) {
    //TODO we assume that this->x and this->y are top left corner and that bounds->x and bounds->y are top left corner
    if (hit) {
        x = CLAMP(e.x - mouseDx, bounds->x, bounds->x + bounds->width - width);
        y = CLAMP(e.y - mouseDy, bounds->y, bounds->y + bounds->height - height);
    }
}
