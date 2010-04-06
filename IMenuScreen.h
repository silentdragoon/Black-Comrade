
#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "menuType.h"

using namespace Ogre;

class IMenuScreen {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual bool visible() = 0;

    virtual MenuType nextMenu() = 0;
};

#endif
