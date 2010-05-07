#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "ITickable.h"
#include "inputState.h"


class Screenshot : public ITickable {
    private:
        int num;
        InputState *inputState;
        Ogre::RenderWindow *window;
        std::string createUnique(std::string lol);
    
    public:
        Screenshot(InputState *inputState, Ogre::RenderWindow *window);
        ~Screenshot();
        virtual void tick();
 
};

#endif
