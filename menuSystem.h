#ifndef MENU_SYSTEM_H 
#define MENU_SYSTEM_H

#include "menuType.h"
#include "IMenuScreen.h"
#include "guiManager.h"
#include "inputState.h"
#include "IExit.h"
#include "stateUpdate.h"

using namespace Ogre;
using namespace std;

class MenuSystem : public OIS::KeyListener, public OIS::MouseListener, public IExit
{
    public:

    MenuSystem(SceneManager *sceneMgr, GuiManager *guiMgr,
               InputState *inputState, RenderWindow *window);

    void run();
    void shutdown();
    void exit();

    void render();

    // Raw input handlers that we care about
    bool mouseMoved(const OIS::MouseEvent &e);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    private:
    void init();

    protected:

    SceneManager *sceneMgr;
    GuiManager *guiMgr;
    InputState *inputState;
    Ogre::RenderWindow *window;

    StateUpdate *menuLoop;
    IMenuScreen *currentMenuScreen;

    bool fadingOut;
    bool fadingIn;

    // convert an OIS mouse button into a CEGUI mouse button
    CEGUI::MouseButton convertOISButtonToCegui(int buttonID);
};

#endif
