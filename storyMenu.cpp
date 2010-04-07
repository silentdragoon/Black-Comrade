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

bool StoryMenu::proceedClicked(const CEGUI::EventArgs& e) {
    if (!nickBox->getText().empty()) {
        networkingMgr->nick = nickBox->getText().c_str();
        isEnd = true;
    }
    return true;
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
    float nameWidth =  namePlacement.getWidth() * wpixel;
    float nameHeight =  namePlacement.getHeight() * hpixel;
    float nameX =  namePlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float nameY =  namePlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    nickBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,nameWidth),CEGUI::UDim(0,nameHeight)));
    nickBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0,nameX),CEGUI::UDim(0,nameY)));
    nickBox->setMaxTextLength(10);
    nickBox->setText("Player");
    nickBox->setSelection(0,nickBox->getText().length());
    nickBox->activate();

    CEGUI::Image buttonPlacement = imageSet->getImage("ProceedButton");
    float buttonWidth =  buttonPlacement.getWidth() * wpixel;
    float buttonHeight =  buttonPlacement.getHeight() * wpixel;
    float buttonX =  buttonPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float buttonY =  buttonPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    CEGUI::PushButton *btn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/ImageButton"));
    guiMgr->getRootWindow()->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,buttonX),CEGUI::UDim(0,buttonY)));
    btn->setSize(CEGUI::UVector2(CEGUI::UDim(0,buttonWidth),CEGUI::UDim(0,buttonHeight)));
    btn->setProperty(CEGUI::String("NormalImage"),CEGUI::String("set:Story image:ProceedButton"));

    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&StoryMenu::proceedClicked, this));

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
