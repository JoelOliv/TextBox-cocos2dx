
#include "pch.h"
#include "DrawSprite.h"

QXRectangle::QXRectangle(){
	_recWidth = 1;
	_recHeight = 1;
	_recColor = ccc4(255,255,255,255);
	QXRectangle(_recWidth, _recHeight, _recColor);
}

QXRectangle::QXRectangle(int width, int height, ccColor4B color){
	_recWidth = width;
	_recHeight = height;
	_recColor = color;

	unsigned char* newdata = new unsigned char[width*height * 4];

	for (int i = 0; i < width*height * 4; i = i + 4){
		unsigned char* newpixel = newdata + i;

		*newpixel = color.r;
		*(newpixel + 1) = color.g;
		*(newpixel + 2) = color.b;
		*(newpixel + 3) = color.a; 
	}

	cocos2d::CCTexture2D* tex = new cocos2d::CCTexture2D();
	tex->initWithData(newdata, kCCTexture2DPixelFormat_Default, width, height, cocos2d::CCSize(width, height));
	
	initWithTexture(tex);
}

void QXRectangle::setQXRectangleWidth(int width){
	_recWidth = width;
	QXRectangle(_recWidth, _recHeight, _recColor);
}

void QXRectangle::setQXRectangleHeight(int height){
	_recHeight = height;
	QXRectangle(_recWidth, _recHeight, _recColor);
}

void QXRectangle::setQXRectangleColor(ccColor4B color){
	_recColor = color;
	QXRectangle(_recWidth, _recHeight, _recColor);
}

int QXRectangle::getQXRectangleWidth(){
	return _recWidth;
}

int QXRectangle::getQXRectangleHeight(){
	return _recHeight;
}

ccColor4B QXRectangle::getQXRectangleColor(){
	return _recColor;
}

