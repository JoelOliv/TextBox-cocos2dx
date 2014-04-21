#include "pch.h"
#include "Objects2dFactory.h"
#include "DrawSprite.h"

using namespace cocos2d;

QXRectangle* Objects2dFactory::rectangle(int width, int height, ccColor4B color){
	QXRectangle* rect = new QXRectangle(width, height, color);
	return rect;
}