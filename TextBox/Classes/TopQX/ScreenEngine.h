//
//  ScreenEngine.h
//  Vox4All
//
//  Created by TopQX 02 on 05/03/13.
//
//

#ifndef Vox4All_ScreenEngine_h
#define Vox4All_ScreenEngine_h

#include "Export.h"
#include <stddef.h>

namespace CocosDenshion {

class EXPORT_DLL ScreenEngine
{
public:
    ScreenEngine();
    ~ScreenEngine();
    
    /**
     @brief Get the shared Engine object,it will new one when first time be called
     */
    static ScreenEngine* sharedEngine();
    
    /**
     @brief Release the shared Engine object
     @warning It must be called before the application exit, or a memroy leak will be casued.
     */
    static void end();
    
	static int getKeyboardHeight();
};

}

#endif
