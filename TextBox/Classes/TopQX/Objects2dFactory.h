#ifndef OBJECTS2DFACTORY_H
#define OBJECTS2DFACTORY_H

#include "pch.h"
#include "cocos2d.h"
#include <string>
#include "Export.h"
#include "DrawSprite.h"

using namespace std;
class QXRectangle;

class EXPORT_DLL Objects2dFactory
{
public:
	static QXRectangle* rectangle(int width, int height, cocos2d::ccColor4B color = cocos2d::ccc4(255, 255, 255, 255));

};

#endif // OBJECTS2DFACTORY_H