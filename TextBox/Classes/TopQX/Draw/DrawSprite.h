#ifndef DRAWSPRITES_H
#define DRAWSPRITES_H

#include "pch.h"
#include "cocos2d.h"
#include "Export.h"

using namespace cocos2d;

class EXPORT_DLL QXRectangle : public CCSprite
{
public:
	QXRectangle(void);
	QXRectangle(int width, int height, ccColor4B color = ccc4(255, 255, 255, 255));

	/*static QXRectangle* create(int width, int height, ccColor4B color = ccc4(255, 255, 255, 255));*/

	void setQXRectangleWidth(int width);
	void setQXRectangleHeight(int height);
	void setQXRectangleColor(ccColor4B color);

	int getQXRectangleWidth();
	int getQXRectangleHeight();
	ccColor4B getQXRectangleColor();
private:
	int _recWidth;
	int _recHeight;
	ccColor4B _recColor;

};
#endif // DRAWSPRITES_H

