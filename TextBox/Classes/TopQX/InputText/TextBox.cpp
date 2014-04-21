#include "pch.h"
#include "TextBox.h"
#include "ScreenEngine.h"
#include "Objects2dHandler.h"

using namespace std;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;

QXTextBox::QXTextBox(float width, float height, ccColor4B boxColor, ccColor4B borderColor, float perBorder, float perSizeFont, std::string placeHolderText, std::string fontName, ccColor3B textColor, bool isPassword, int maxnumberOfChars){
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	int w = width;// / visibleSize.width * 2048;
	int h = height;// / visibleSize.height * 1536;

	QXRectangle* borderBox = Objects2dFactory::rectangle(w, h, borderColor);
	borderBox->setAnchorPoint(ccp(0.5f, 0.5f));
	float border = borderBox->getContentSize().height * perBorder;
	QXRectangle* result2 = Objects2dFactory::rectangle(borderBox->getContentSize().width - 2 * border, borderBox->getContentSize().height - 2 * border);
	result2->setAnchorPoint(ccp(0.5f, 0.5f));
	result2->setPosition(ccp(borderBox->getContentSize().width / 2, borderBox->getContentSize().height / 2));
	borderBox->addChild(result2);


	create();
	initWithNormalSprite(borderBox, borderBox, borderBox, this, menu_selector(QXTextBox::menuIMECallback));
	setAnchorPoint(ccp(0, 0));
	m_pTextBox = new QXTextBoxField(result2->getContentSize().width, result2->getContentSize().height, perSizeFont, placeHolderText, fontName);
	//(QXTextBoxField* )CCTextFieldTTF::textFieldWithPlaceHolder("click here for input", CCSize(result2->getContentSize().width - result2->getContentSize().height * (1 - perSizeFont), result2->getContentSize().height), CCTextAlignment::kCCTextAlignmentLeft, "Thonburi", result2->getContentSize().height*perSizeFont);
	m_pTextBox->setFontName(fontName.c_str());
	m_pTextBox->setColor(textColor);
	m_pTextBox->setAnchorPoint(ccp(0.f, 0.f));
	m_pTextBox->setPosition(ccp(result2->getContentSize().height*(1 - perSizeFont) / 2, 0));
	this->addChild(m_pTextBox);
}

void QXTextBox::menuIMECallback(CCObject* pSender){
	m_pTextBox->attachWithIME();
}

QXTextBoxField::QXTextBoxField(float width, float height, float perSizeFont, std::string placeHolderText, std::string fontName, bool isPassword, int maxnumberOfChars){
	// Delimiter character
	_delimiter_char = '|';

	// Maximum number of characters
	_max_chars = maxnumberOfChars;

	// Is editable?
	_is_active =  false;

	// Current complete text
	_hiddenText.append("");

	// Is a password
	mIsPassword = isPassword;

	initWithPlaceHolder(placeHolderText.c_str(), CCSize(width - height * (1 - perSizeFont), height), CCTextAlignment::kCCTextAlignmentLeft, fontName.c_str(), height*perSizeFont);
	setString("");
	setDelegate(new QXTextBoxFieldDelegate());
	


}

void QXTextBoxField::setString(const char *text){
	string inputText = string(text);

	// insert \n means input end
	//int nPos = sInsert.find('\n');
	int nPos = inputText.find('\n');
	if ((int)inputText.npos != nPos)
	{
		inputText.erase(nPos);
		detachWithIME();
		return;
	}

	if (!mIsPassword){
		// Isn't active?
		if (!_is_active)
		{
			// Call parent set string, which defines if it necessary to show place holder or not
			//string inputText = string(text);
			CCTextFieldTTF::setString(inputText.c_str()); // Sometimes text point to the same location that m_pInputText; so, if it is sent to set string (where m_pInputText is deleted), it is deleted
			return;
		}


		// Length of new string, if we want to remove characters
		int newStringNumChars = _max_chars;

		// Add space for characters whose length is more than 1
		for (int i = 0; i < strlen(text) && i <= newStringNumChars /* Don't want to count extra characters after the end of the string */; i++)
		{
			if (0x80 == (0xC0 & inputText.at(i)))
				newStringNumChars++;
		}

		// If it has more than the allowed characters
		if (strlen(text) > newStringNumChars)
		{
			// Characteres after the maximum characters allowed are removed
			inputText = inputText.substr(0, newStringNumChars);
		}

		// Update m_pInputText
		m_pInputText = new std::string(inputText.c_str());

		// Add delimiter character
		string textStr = string(m_pInputText->c_str());
		textStr += _delimiter_char;

		// Update string
		CCLabelTTF::setString(textStr.c_str());

		// Update number of characters
		m_nCharCount = strlen(m_pInputText->c_str());

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
		string delimiter;
		delimiter.push_back(this->_delimiter_char);

		string currentText = getString();
		string completeText = _hiddenText;
		completeText.append(currentText);

		_hiddenText.clear();
		currentText.clear();

		// Current text width calculation
		int pos = completeText.length() - 1;
		CCLabelTTF *widthMetterAux;
		float calculatedWidth = 0.0;
		float maxWidth = this->getDimensions().width - spaceAfterBorder;
		bool itFit = false;

		// Starting with only last character, this cycle adds characteres one by one in the beggining of the
		// string and calculates the width. If doesn't exceeds the widht limit, adds another character.
		// In the end of the cycle, last position (pos) divides hidden text from showed text
		while (calculatedWidth < maxWidth)
		{
			if (pos < 0) {
				itFit = true;
				break;
			}

			currentText.clear();
			currentText.append(completeText.substr(pos));
			currentText.append(delimiter);
			widthMetterAux = CCLabelTTF::create(currentText.c_str(), this->getFontName(), this->getFontSize());
			calculatedWidth = widthMetterAux->getTexture()->getContentSizeInPixels().width;

			pos--;
		}

		if (itFit) {
			_hiddenText.clear();
			currentText.clear();
			currentText.append(completeText);
		}
		else {
			pos += 2;
			_hiddenText.append(completeText.substr(0, pos));
			currentText.clear();
			currentText.append(completeText.substr(pos, completeText.length() - 1));
		}

		// Update m_pInputText
		m_pInputText = new std::string(currentText.c_str());

		// Add delimiter character
		string textStr2 = string(m_pInputText->c_str());
		textStr2 += _delimiter_char;

		// Update string
		CCLabelTTF::setString(textStr2.c_str());

		// Update number of characters
		m_nCharCount = strlen(m_pInputText->c_str());
#endif
	}
	else{
		// Isn't active?
		if (!_is_active && string(text) == "")
		{
			// Call parent set string, which defines if it necessary to show place holder or not
			//string inputText = string(text);
			CCTextFieldTTF::setString(inputText.c_str()); // Sometimes text point to the same location that m_pInputText; so, if it is sent to set string (where m_pInputText is deleted), it is deleted
			return;
		}

		// If it has more than the allowed characters
		if (strlen(text) > _max_chars)
		{
			// Characteres after the maximum characters allowed are removed
			string textStrAux = string(text).substr(0, _max_chars);
			text = new char[textStrAux.length() + 1];
			text = textStrAux.c_str();
		}

		// Update m_pInputText
		m_pInputText = new std::string(text);

		//begin code modification
		inputText = string(m_pInputText->c_str());
		if (mIsPassword)
		{
			int length = strlen(inputText.c_str());

			inputText = fillWithStars(length);
		}
		//end code modification

		// Is active?
		if (_is_active)
		{
			// Add delimiter character
			inputText += _delimiter_char;
		}

		// Update string
		CCLabelTTF::setString(inputText.c_str());

		m_nCharCount = strlen(m_pInputText->c_str());
	}
}

void QXTextBoxField::deleteBackward(){
	CCTextFieldTTF::deleteBackward();
	string x = getString();
	setString(x.c_str());

}


string QXTextBoxField::fillWithStars(int length){
	stringstream str;
	str.clear();
	for (int i = 0; i<length; i++)
	{
		str << "*";
	}
	return str.str();
}

void QXTextBoxField::setPassword(bool isPassword){
	mIsPassword = isPassword;
}

void QXTextBoxField::changeScene(float dt)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
	// Wait 0,3 seconds
	//usleep(300000);

	// Get keyboard top
	int _keyboard_top = CocosDenshion::ScreenEngine::sharedEngine()->getKeyboardHeight();
#endif
	// Textbox bottom
	float textboxBottom = (getPosition().y - getAnchorPoint().y * boundingBox().size.height);
	CCPoint p = ccp(0, textboxBottom);
	p = convertToWorldSpace(p);
	textboxBottom = p.y;

	// Increment scroll position
	//CCSprite* scrollParent = dynamic_cast<CCSprite*>(getParent());
	//if (scrollParent && dynamic_cast<ScrollableZone*>(getParent()->getParent()) /* Its parent is a scrollable zone */)
	//{
	//	textboxBottom += (getParent()->getPosition().y - getParent()->getAnchorPoint().y * getParent()->boundingBox().size.height);
	//}

	// If keyboard hides textbox
	if (_keyboard_top > textboxBottom)
	{
		// Change scene position
		CCDirector::sharedDirector()->getRunningScene()->setPositionY(_keyboard_top - textboxBottom + CCDirector::sharedDirector()->getWinSize().height * 0.02f);
	}
}

void QXTextBoxField::changeSceneDown(float dt)
{
	if (CocosDenshion::ScreenEngine::sharedEngine()->getKeyboardHeight() > 0)
	{
	}
	else{
		detachWithIME();
		getScheduler()->unscheduleAll();
	}
}

//// On enter event
//void QXTextBoxField::onEnter(){
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((CCTargetedTouchDelegate*)this, kCCMenuHandlerPriority - 9, false); // kCCMenuHandlerPriority - 1 has a higher priority than any CCMenu
//
//	CCNode::onEnter();
//}
//
////On exit event
//void QXTextBoxField::onExit(){
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate((CCTargetedTouchDelegate*)this);
//
//	CCNode::onExit();
//}
//
//// Touch ended event
//bool QXTextBoxField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//	CCSize size = CCDirector::sharedDirector()->getWinSize();
//
//	// Final position
//	CCPoint* finalPosition = new CCPoint(pTouch->getLocationInView().x, size.height - pTouch->getLocationInView().y);
//	//*finalPosition = this->getParent()->convertToNodeSpace(*finalPosition);
//
//	// If it is active and user clicked outside text box
//	if (_is_active && !Objects2dHandler::isPositionInsideNode(finalPosition, this))
//	{
//		detachWithIME();
//		CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(kCCMenuHandlerPriority - 9, (CCTargetedTouchDelegate*)this);
//	}
//	// If it is not active and user clicked inside text box
//	else if (!_is_active && Objects2dHandler::isPositionInsideNode(finalPosition, this))
//	{
//		attachWithIME();
//		CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(kCCMenuHandlerPriority - 10, (CCTargetedTouchDelegate*)this);
//	}
//
//	// Call other touch events
//	return false;
//}
//
//
//void QXTextBoxField::keyboardWillShow(CCIMEKeyboardNotificationInfo& info){
//	
//}
//
//void QXTextBoxField::keyboardDidHide(CCIMEKeyboardNotificationInfo& info){
//	if (_is_active)
//		detachWithIME();
//}

bool QXTextBoxField::attachWithIME(){
	// Update state
	_is_active = true;

	// Update input
	setString(getString());


	return this->CCTextFieldTTF::attachWithIME();
}

bool QXTextBoxField::detachWithIME(){
	// Update state
	_is_active = false;

	// Update input
	setString(getString());

	// Reset scene position
	CCDirector::sharedDirector()->getRunningScene()->setPositionY(0);
	//CCTextFieldTTF * pTextField = (CCTextFieldTTF*)this;
	return this->CCTextFieldTTF::detachWithIME();
}

bool QXTextBoxFieldDelegate::onTextFieldAttachWithIME(CCTextFieldTTF * pSender){

	CCMenu* menu = (CCMenu*)pSender->getParent()->getParent();
	CCArray* items = menu->getChildren();
	int num_items = menu->getChildrenCount();
	for (int i = 0; i < num_items; i++){
		QXTextBoxField* aux = ((QXTextBox*)items->objectAtIndex(i))->m_pTextBox;
		if (aux != pSender){
			aux->_is_active = false;
		}
	}
	((QXTextBoxField*)pSender)->getScheduler()->unscheduleAll();
	((QXTextBoxField*)pSender)->scheduleOnce(schedule_selector(QXTextBoxField::changeScene), 0.50f);
	((QXTextBoxField*)pSender)->schedule(schedule_selector(QXTextBoxField::changeSceneDown), 1.0f);
	return false;
}

bool QXTextBoxFieldDelegate::onTextFieldDetachWithIME(CCTextFieldTTF * pSender){

	// Update input
	((QXTextBoxField*)pSender)->_is_active = false;
	((QXTextBoxField*)pSender)->setString(((QXTextBoxField*)pSender)->getString());
	return false;
}

bool QXTextBoxFieldDelegate::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen){

	// Update input
	
	((QXTextBoxField*)pSender)->setString(((QXTextBoxField*)pSender)->getString());
	return false;
}

bool QXTextBoxFieldDelegate::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen){

	// Update input
	((QXTextBoxField*)pSender)->setString(((QXTextBoxField*)pSender)->getString());
	return false;
}

bool QXTextBoxFieldDelegate::onDraw(CCTextFieldTTF * pSender){


	return false;
}
