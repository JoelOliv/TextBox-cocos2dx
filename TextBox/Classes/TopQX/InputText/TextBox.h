#ifndef QXTextBox_H
#define QXTextBox_H

#include "pch.h"
#include "cocos2d.h"
#include "Export.h"
#include "DrawSprite.h"
//#include "TextBoxField.h"
#include "Objects2dFactory.h"

using namespace std;
using namespace cocos2d;

class QXTextBoxField;
class QXTextBoxFieldDelegate;

class EXPORT_DLL QXTextBox : public CCMenuItemImage, public CCIMEDelegate
{
	
public:
	QXTextBox(float width, float height, ccColor4B boxColor, ccColor4B borderColor, float perBorder, float perSizeFont, std::string placeHolderText, std::string fontName="Arial", ccColor3B textColor = ccc3(255,255,255), bool isPassword = false, int maxnumberOfChars = 999);

	void menuIMECallback(CCObject* pSender);
	QXTextBoxField*    m_pTextBox;

};

class EXPORT_DLL QXTextBoxField : public CCTextFieldTTF, public CCIMEDelegate{
public:
	QXTextBoxField(float width, float height, float perSizeFont, std::string placeHolderText, std::string fontName = "Arial", bool isPassword = false, int maxnumberOfChars = 999);

	// Set string
	virtual void setString(const char *text);
	virtual void deleteBackward();
	std::string fillWithStars(int length);

	void setPassword(bool isPassword);
	void changeScene(float dt);
	void changeSceneDown(float dt);

	//// On enter event
	//virtual void onEnter();

	////On exit event
	//virtual void onExit();

	//// Touch ended event
	//virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);


	//void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);

	//void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);

	virtual bool attachWithIME();

	virtual bool detachWithIME();

	virtual bool canDetachWithIME(){
		return CCTextFieldTTF::canDetachWithIME();
	};

	// Is editable?
	bool _is_active;

protected:
	// Delimiter character
	char _delimiter_char;

	// Maximum number of characters
	int _max_chars;



	// Current complete text
	std::string _hiddenText;

	// Is a password
	bool mIsPassword;

	float spaceAfterBorder;

};

class EXPORT_DLL QXTextBoxFieldDelegate: public CCTextFieldDelegate
{
public:
	// CCTextFieldDelegate
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
	virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
	virtual bool onDraw(CCTextFieldTTF * pSender);
};

#endif	// QXTextBox_H
