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
    imageSet = &(CEGUI::ImagesetManager::getSingleton().create("story.xml"));
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

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float wpixel = winWidth / 1680.0;
    float hpixel = winHeight / 1050.0;

    guiMgr->addStaticImage("Story",0.5, 0.5,1.0, 1.0,"Story","Whole");

    nickBox = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","nickBox"));
    guiMgr->getRootWindow()->addChildWindow(nickBox);

    CEGUI::Image namePlacement = imageSet->getImage("NamePlacement");
    float nameWidth =  namePlacement.getWidth();
    float nameHeight =  namePlacement.getHeight();
    float nameX =  namePlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float nameY =  namePlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    nickBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,nameWidth),CEGUI::UDim(0,nameHeight)));
    nickBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0,nameX-3),CEGUI::UDim(0,nameY-3)));
    nickBox->setMaxTextLength(10);
    nickBox->setText("Player");
    nickBox->setSelection(0,nickBox->getText().length());
    nickBox->activate();

    isVisible = true;
}

void StoryMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Story");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NamePlacement");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(nickBox);
}

bool StoryMenu::end() { return isEnd; }

bool StoryMenu::visible() { return isVisible; }
