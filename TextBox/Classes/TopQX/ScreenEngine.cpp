#include "pch.h"
#include "ScreenEngine.h"


using namespace Platform;
using namespace Windows::UI::ViewManagement;

namespace CocosDenshion {
    
	static ScreenEngine *s_pEngine = 0;
    
	ScreenEngine::ScreenEngine()
	{
        
	}
    
	ScreenEngine::~ScreenEngine()
	{
        
	}
    
	ScreenEngine* ScreenEngine::sharedEngine()
	{
		if (! s_pEngine)
		{
			s_pEngine = new ScreenEngine();
		}
        
		return s_pEngine;
	}
    
	void ScreenEngine::end()
	{
		
	}
    
	int ScreenEngine::getKeyboardHeight()
	{
		auto inputPane = InputPane::GetForCurrentView();
		return inputPane->OccludedRect.Height;
	}
}