#include "statsScreen.h"
#include "IMenuScreen.h"

StatsScreen::StatsScreen(InputState *inputState, GuiManager *guiMgr,
                         CollaborationInfo *pilotInfo,
                         CollaborationInfo *navInfo,
                         CollaborationInfo *engInfo)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , isEnd(false)
    , isVisible(false)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
{
    //CEGUI::ImagesetManager::getSingleton().create("LoadingScreen.imageset");
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

    float g = (1.0*winWidth)/1680.0;
    wpx = 1.0 / (float)winWidth * g;
    hpx = 1.0 / (float)winHeight * g;
    isVisible = false;
}

void StatsScreen::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_SPACE)) {
        isEnd = true;
    }
}

MenuType::StatsScreen::nextMenu() {
    return MT_NONE;
}

void StatsScreen::show() {
    // Show background image etc
    if (isVisible) return;

    std::stringstream out;

    for (int i = 0; i < 130; i ++) {
        out << '*';
    }
    guiMgr->addStaticImage("Scores",0.5, 0.5,1.0, 1.0,"Scores","Whole");

    //guiMgr->addStaticText("StatsHeader", "Stats",0.5, 0.25, 1);
    //guiMgr->addStaticText("StatsUnderline",out.str(),0.5,0.30, 1);

    //guiMgr->addStaticText("PilotHeader", "Pilot",0.4, 0.35, 1);
    //guiMgr->addStaticText("NavHeader", "Engineer",0.6, 0.35, 1);
    //guiMgr->addStaticText("EngHeader", "Navigator",0.8, 0.35, 1);

    //guiMgr->addStaticText("ShotsFiredHeader", "Shots fired", 0.2, 0.4, 1);
    //guiMgr->addStaticText("OnTargetHeader", "...on target", 0.2, 0.45, 1);
    //guiMgr->addStaticText("EnemiesDestroyedHeader", "Enemies destroyed", 0.2, 0.5, 1);

    out.str("");
    out << navInfo->getPlayerStats()->shotsFired;
    guiMgr->addStaticText("NavShotsFired", out.str(), 725*wpx, 0.4, 1);
    out.str("");
    out << navInfo->getPlayerStats()->shotsHit;
    guiMgr->addStaticText("NavOnTarget", out.str(), 725*wpx, 0.45, 1);
    out.str("");
    out << navInfo->getPlayerStats()->enemiesDestroyed;
    guiMgr->addStaticText("NavEnemiesDestroyed", out.str(), 725*wpx, 0.5, 1);

    out.str("");
    out << pilotInfo->getPlayerStats()->shotsFired;
    guiMgr->addStaticText("PilotShotsFired", out.str(), 1025*wpx, 0.4, 1);
    out.str("");
    out << pilotInfo->getPlayerStats()->shotsHit;
    guiMgr->addStaticText("PilotOnTarget", out.str(), 1025*wpx, 0.45, 1);
    out.str("");
    out << pilotInfo->getPlayerStats()->enemiesDestroyed;
    guiMgr->addStaticText("PilotEnemiesDestroyed", out.str(), 1025*wpx, 0.5, 1);

    out.str("");
    out << engInfo->getPlayerStats()->shotsFired;
    guiMgr->addStaticText("EngShotsFired", out.str(), 1325*wpx, 0.4, 1);
    out.str("");
    out << engInfo->getPlayerStats()->shotsHit;
    guiMgr->addStaticText("EngOnTarget", out.str(), 1325*wpx, 0.45, 1);
    out.str("");
    out << engInfo->getPlayerStats()->enemiesDestroyed;
    guiMgr->addStaticText("EngEnemiesDestroyed", out.str(), 1325*wpx, 0.5, 1);

    //guiMgr->addStaticText("ExitInstructions", "Press the space bar to exit...", 0.5, 0.7, 1);

    isVisible = true;
}

void StatsScreen::hide() {
    // Hide background image etc
}

bool StatsScreen::end() { return isEnd; }

bool StatsScreen::visible() { return isVisible; }
