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
    , maxRating(10)
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
        out << (double) stats->shotsFired / stats->shotsHit << "%";
    } else {
        out << "0 %";
    }
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.45, 1);
    out.str("");
    out << stats->enemiesDestroyed;
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.5, 1);

    out.str("");
    out << stats->repairsMade;
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.55, 1);

    if (info->getGameRole() == PILOT) {
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
    out << calcIndividualRating(info);
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.7, 1);
}

void StatsScreen::addOverallRating() {
    guiMgr->addStaticText("", calcOverallRating(), 1025*wpx, 0.78, 1);
}

std::string StatsScreen::calcIndividualRating(CollaborationInfo *info) {
    // TODO: Calculate individual rating
    double rating = 0.0;
    PlayerStats *stats = info->getPlayerStats();

    double wallHitWeight, speedWeight,
        accuracyWeight, repairsWeight,
        destroyedWeight;

    int maxWallHits = 100;
    int maxAvgSpeed = 350;
    int maxDestroyed = 200;

    if (info->getGameRole() == PILOT) {
        
        wallHitWeight = 0.4;
        speedWeight = 0.25;
        accuracyWeight = 0.1;
        destroyedWeight = 0.05;
        repairsWeight = 0.2;

    } else {
        wallHitWeight = 0.0;
        speedWeight = 0.0;
        destroyedWeight = 0.2;
        accuracyWeight = 0.5;
        repairsWeight = 0.3;
    }

    double accuracy;
    if (stats->shotsHit != 0) {
        accuracy = stats->shotsFired / (double) stats->shotsHit;
    } else {
        accuracy = 0.0;
    }

    int speedComp = (stats->averageSpeed == 0) ?
                    0 : (maxRating*speedWeight) * (maxAvgSpeed/stats->averageSpeed);
    int collisionsComp = (stats->numCollisions == 0) ?
                    (maxRating*wallHitWeight) : (maxRating*wallHitWeight) * (maxWallHits /  stats->numCollisions);
    int destroyedComp = (stats->enemiesDestroyed == 0) ? 0 : (maxRating*destroyedWeight) * (1 - (maxDestroyed /  stats->enemiesDestroyed));
    int repairsComp = (stats->repairsMade == 0) ? 0 : (maxRating*repairsWeight) * (1 - (8 /  stats->repairsMade));
    int accuracyComp = (maxRating*accuracyWeight) * accuracy;

    rating =  speedComp + collisionsComp + destroyedComp + repairsComp + accuracyComp;

    info->getPlayerStats()->overallRating = rating;
    if (rating < (maxRating/4.0)) {
        return "D";
    } else if (rating < (maxRating/4.0)*2) {
        return "C";
    } else if (rating < (maxRating/4.0)*3) {
        return "B";
    } else { return "A"; }
}

std::string StatsScreen::calcOverallRating() {
    double pilotRating = pilotInfo->getPlayerStats()->overallRating;
    double engRating = engInfo->getPlayerStats()->overallRating;
    double navRating = navInfo->getPlayerStats()->overallRating;
    int overall = ((pilotRating + engRating + navRating) / 3.0);

    if (overall < (maxRating/4.0)) {
        return "D";
    } else if (overall < (maxRating/4.0)*2) {
        return "C";
    } else if (overall < (maxRating/4.0)*3) {
        return "B";
    } else { return "A"; }
}

void StatsScreen::hide() {
    // Hide background image etc
}

bool StatsScreen::end() { return isEnd; }

bool StatsScreen::visible() { return isVisible; }
