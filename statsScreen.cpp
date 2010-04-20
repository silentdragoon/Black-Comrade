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

    //if (finishState == GS_END) {
        CEGUI::ImagesetManager::getSingleton().create("scores.xml");
        guiMgr->addStaticImage("Scores",0.5, 0.5,1.0, 1.0,"Scores","Whole");
        addStats(navInfo,725);
    	addStats(pilotInfo,1025);
    	addStats(engInfo,1325);
	    addOverallRating();
    //} else {
    //    CEGUI::ImagesetManager::getSingleton().create("scoresfail.xml");
    //    guiMgr->addStaticImage("ScoresFail",0.5, 0.5,1.0, 1.0,"ScoresFail","Whole");
    //}



    isVisible = true;
}

void StatsScreen::addStats(CollaborationInfo *info, int columnOffset) {
    PlayerStats *stats = info->getPlayerStats();
    std::stringstream out;
    out << stats->shotsFired;
    guiMgr->addStaticText("", out.str(), columnOffset*wpx, 0.4, 1);
    out.str("");
    if (stats->shotsHit != 0) {
        out << ((double) stats->shotsHit / stats->shotsFired) * 100 << "%";
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

    int maxWallHits = 50;
    int maxAvgSpeed = 300;
    int maxDestroyed = 200;
    stats->averageSpeed = (stats->averageSpeed > maxAvgSpeed) ? maxAvgSpeed : stats->averageSpeed;
    stats->numCollisions = (stats->numCollisions > maxWallHits) ? maxWallHits : stats->numCollisions;
    stats->enemiesDestroyed = (stats->enemiesDestroyed > maxDestroyed) ? maxDestroyed : stats->enemiesDestroyed;

    if (info->getGameRole() == PILOT) {
        //std::cout << "PILOT:\n";
        wallHitWeight = 0.2;
        speedWeight = 0.5;
        accuracyWeight = 0.1;
        destroyedWeight = 0.05;
        repairsWeight = 0.15;

    } else {
        //std::cout << "OTHER:\n";
        wallHitWeight = 0.0;
        speedWeight = 0.0;
        destroyedWeight = 0.35;
        accuracyWeight = 0.35;
        repairsWeight = 0.3;
    }

    double accuracy;
    if (stats->shotsHit != 0) {
        accuracy = (double) stats->shotsHit / stats->shotsFired;
    } else {
        accuracy = 0.0;
    }

    double speedComp = (stats->averageSpeed == 0) ?
                    0 : (maxRating*speedWeight) * (stats->averageSpeed / maxAvgSpeed);
    double collisionsComp = (stats->numCollisions == 0) ?
                    (maxRating*wallHitWeight) : (maxRating*wallHitWeight) * 1-(stats->numCollisions / maxWallHits);
    double destroyedComp = (stats->enemiesDestroyed == 0) ? 0 : (maxRating*destroyedWeight) * (stats->enemiesDestroyed / (double) maxDestroyed);
    double repairsComp = (stats->repairsMade == 0) ? 0 : (maxRating*repairsWeight) * (1-(20 /  stats->repairsMade));
    double accuracyComp = (maxRating*accuracyWeight) * accuracy;

    //std::cout << speedComp << "\n";
    //std::cout << collisionsComp << "\n";
    //std::cout << destroyedComp << "\n";
    //std::cout << repairsComp << "\n";
    //std::cout << accuracyComp << "\n";

    rating =  speedComp + collisionsComp + destroyedComp + repairsComp + accuracyComp;
    //std::cout << rating << "\n";

    info->getPlayerStats()->overallRating = rating;
    if (rating <= (maxRating/4.0)) {
        return "D";
    } else if (rating <= (maxRating/4.0)*2) {
        return "C";
    } else if (rating <= (maxRating/4.0)*3) {
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
