
#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "preGameMenu.h"
#include "IConsoleInputReceiver.h"

using namespace Ogre;

class IMenuScreen : public IConsoleInputReceiver {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual bool visible() = 0;

    virtual PreGameMenu nextMenu() = 0;

    virtual void alphaNumKeyPressed(const OIS::KeyEvent &arg) {};
    virtual void returnKeyPressed() {};
    virtual void backspaceKeyPressed() {};
};

#endif
