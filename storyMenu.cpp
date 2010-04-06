#include "storyMenu.h"

StoryMenu::StoryMenu(InputState *inputState,
                           NetworkingManager *networkingMgr,
                           GuiManager *guiMgr)
    : inputState(inputState)
    , networkingMgr(networkingMgr)
    , guiMgr(guiMgr)
    , isVisible(false)
    , isEnd(false)
{
    CEGUI::ImagesetManager::getSingleton().create("story.xml");
}

void StoryMenu::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_RETURN)) {
        if (!nickBox->getText().empty()) {
            networkingMgr->nick = nickBox->getText().c_str();
            isEnd = true;
        }
    } else if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(0);
    }
}

MenuType StoryMenu::nextMenu() {
    return MT_CHOOSE_NETWORK_ROLE;
}

void StoryMenu::show() {
    // Show background image etc
    if (isVisible) return;

    guiMgr->addStaticImage("Story",0.5, 0.5,1.0, 1.0,"Story","Whole");

    nickBox = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","nickBox"));
    guiMgr->getRootWindow()->addChildWindow(nickBox);
    nickBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,100),CEGUI::UDim(0,100)));
    nickBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0),CEGUI::UDim(0.5,0)));
    nickBox->setMaxTextLength(10);
    nickBox->setText("Player");
    nickBox->setCaratIndex(nickBox->getText().length());
    nickBox->activate();

    isVisible = true;
}

void StoryMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Story");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(nickBox);
}

bool StoryMenu::end() { return isEnd; }

bool StoryMenu::visible() { return isVisible; }
