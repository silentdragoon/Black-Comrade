#ifndef STORY_MENU_H
#define STORY_MENU_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "networkingManager.h"
#include "guiManager.h"
#include "menuType.h"
#include <OIS/OIS.h>

class StoryMenu : public IMenuScreen
{
    private:
        InputState *inputState;
        NetworkingManager *networkingMgr;
        GuiManager *guiMgr;

        CEGUI::Imageset *bgImageSet;
        CEGUI::Imageset *btImageSet;
        CEGUI::Editbox *nickBox;
        CEGUI::PushButton *btn;

        bool isVisible;
        bool isEnd;
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        bool proceedClicked(const CEGUI::EventArgs& e);

        MenuType nextMenu();

        StoryMenu(InputState *inputState, NetworkingManager *networkingMgr, GuiManager *guiMgr);
        ~StoryMenu();
};

#endif
