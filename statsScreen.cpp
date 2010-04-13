#include "statsScreen.h"
#include "IMenuScreen.h"

StatsScreen::StatsScreen(InputState *inputState, GuiManager *guiMgr,
                         CollaborationInfo *pilotInfo,
                         CollaborationInfo *navInfo,
                         CollaborationInfo *engInfo,
                         GameState finishState)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , isEnd(false)
    , isVisible(false)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
    , finishState(finishState)
{

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

    if (finishState == GS_END) {
        CEGUI::ImagesetManager::getSingleton().create("scores.xml");
        guiMgr->addStaticImage("Scores",0.5, 0.5,1.0, 1.0,"Scores","Whole");
    } else {
        CEGUI::ImagesetManager::getSingleton().create("scoresfail.xml");
        guiMgr->addStaticImage("ScoresFail",0.5, 0.5,1.0, 1.0,"ScoresFail","Whole");
    }

    addStats(navInfo,725);
    addStats(pilotInfo,1025);
    addStats(engInfo,1325);

    addOverallRating();

    isVisible = true;
}

void StatsScreen::addStats(CollaborationInfo *info, int columnOffset) {
    PlayerStats *stats = info->getPlayerStats();
    std::stringstream out;
    out << stats->shotsFired;
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.4, 1);
    out.str("");
    if (stats->shotsHit != 0) {
        out << stats->shotsFired / stats->shotsHit << " %";
    } else {
        out << "0 %";
    }
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.45, 1);
    out.str("");
    out << stats->enemiesDestroyed;
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.5, 1);

    out.str("");
    out << "0"; // TODO: Add 'repairs made' metric
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.55, 1);

    if (info->getGameRole() == PILOT) {
        // TODO: Add pilot metric
        out.str("");
        out << (int) stats->averageSpeed << " KM/H";
        guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.6, 1);

        out.str("");
        out << stats->numCollisions;
        guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.65, 1);
    } else {
        out.str("-");
        guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.6, 1);
        guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.65, 1);
    }

    out.str("");
    out << "A"; // TODO: Calculate individual rating
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.7, 1);
}

void StatsScreen::addOverallRating() {
    guiMgr->addStaticText("", "A", 1025*wpx, 0.78, 1); // TODO: Calculate overall rating
}

void StatsScreen::hide() {
    // Hide background image etc
}

bool StatsScreen::end() { return isEnd; }

bool StatsScreen::visible() { return isVisible; }
