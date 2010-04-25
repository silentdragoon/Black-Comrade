#include "statsScreen.h"
#include "IMenuScreen.h"

StatsScreen::StatsScreen(InputState *inputState, GuiManager *guiMgr,
                         CollaborationInfo *pilotInfo,
                         CollaborationInfo *navInfo,
                         CollaborationInfo *engInfo,
                         GameState finishState,
                         int damageSustained,
                         int gameLength)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , isEnd(false)
    , isVisible(false)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
    , finishState(finishState)
    , maxRating(10)
    , delim('\t')
    , damageSustained(damageSustained)
    , gameLength(gameLength)
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
        saveStats("stats");
        loadExistingStats("stats");
        addStats(navInfo,725);
    	addStats(pilotInfo,1025);
    	addStats(engInfo,1325);
	addOverallRating();

        std::cout << "The game lasted " << gameLength << " seconds, and there was " << damageSustained << " damage.\n";

        //std::cout << "Average pilot shots fired: " << getAverage(pilotShots) << "\n";
        //std::cout << "Average average speeds: " << getAverage(averageSpeeds) << "\n";
        //std::cout << "Max average speeds: " << getMax(averageSpeeds) << "\n";
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


void StatsScreen::saveStats(std::string fileName) {
    std::ofstream statsFile;
    statsFile.open (fileName.c_str(),std::ios::app);

    // Write pilot stats
    PlayerStats *stats = pilotInfo->getPlayerStats();
    statsFile << stats->shotsFired << delim;
    statsFile << stats->shotsHit << delim;
    statsFile << stats->enemiesDestroyed << delim;
    statsFile << stats->repairsMade << delim;
    statsFile << stats->averageSpeed << delim;
    statsFile << stats->numCollisions << delim;

    // Write nav stats
    stats = navInfo->getPlayerStats();
    statsFile << stats->shotsFired << delim;
    statsFile << stats->shotsHit << delim;
    statsFile << stats->enemiesDestroyed << delim;
    statsFile << stats->repairsMade << delim;

    // Write eng stats
    stats = engInfo->getPlayerStats();
    statsFile << stats->shotsFired << delim;
    statsFile << stats->shotsHit << delim;
    statsFile << stats->enemiesDestroyed << delim;
    statsFile << stats->repairsMade << delim;

    // Write game stats
    statsFile << gameLength << delim;
    statsFile << damageSustained;

    statsFile << "\n";

    statsFile.close();
}

void StatsScreen::loadExistingStats(std::string fileName) {
    // Read the file
    std::ifstream fin(fileName.c_str());
    int numberOfSavedGames = 0;

    if(fin.is_open()) {
        std::string line;
        while(!fin.eof()) {
            numberOfSavedGames ++;
            std::getline(fin,line);
            
            std::istringstream *iss = &(std::istringstream(line));
            std::string stat;

            // Parse pilot stats

            parseCommonStats(iss,true);

            if (!getline(*iss,stat,delim)) continue;
            averageSpeeds.push_back(toDouble(stat));

            if (!getline(*iss,stat,delim)) continue;
            wallHits.push_back(toInt(stat));

            // Parse the other stats

            parseCommonStats(iss,false);
            parseCommonStats(iss,false);

            if (!getline(*iss,stat,delim)) continue;
            gameLengths.push_back(toDouble(stat));

            if (!getline(*iss,stat,delim)) continue;
            damagesSustained.push_back(toInt(stat));

        }
        
    } else {
        std::cerr << "Unable to open stats file '"
            << fileName << "'" << std::endl;
    }
}

double StatsScreen::getAverage(std::vector<int> stats) {
    if (stats.size() == 0) return 0.0;

    int tot = 0;
    for (int i=0; i < stats.size() ; i++) {
        tot += stats.at(i);
    }
    return ((double) tot) / stats.size();
}

double StatsScreen::getMax(std::vector<double> stats) {
    if (stats.size() == 0) return 0.0;

    double max = 0.0;
    for (int i=0; i < stats.size() ; i++) {
        if (stats.at(i) > max) max = stats.at(i);
    }
    return max;
}

double StatsScreen::getMax(std::vector<int> stats) {
    if (stats.size() == 0) return 0.0;

    int max = 0;
    for (int i=0; i < stats.size() ; i++) {
        if (stats.at(i) > max) max = stats.at(i);
    }
    return max;
}

double StatsScreen::getAverage(std::vector<double> stats) {
    if (stats.size() == 0) return 0.0;

    double tot = 0.0;
    for (int i=0; i < stats.size() ; i++) {
        tot += stats.at(i);
    }
    return tot / stats.size();
}

void StatsScreen::parseCommonStats(std::istringstream *iss, bool pilot) {
    std::string stat;
    std::vector<int> *shots = (pilot) ? &pilotShots : &nonPilotShots;
    std::vector<int> *hits = (pilot) ? &pilotHits : &nonPilotHits;
    std::vector<int> *destroyed = (pilot) ? &pilotDestroyed : &nonPilotDestroyed;
    std::vector<int> *repairs = (pilot) ? &pilotRepairs : &nonPilotRepairs;

    if (!getline(*iss,stat,delim)) return;
    shots->push_back(toInt(stat));

    if (!getline(*iss,stat,delim)) return;
    hits->push_back(toInt(stat));

    if (!getline(*iss,stat,delim)) return;
    destroyed->push_back(toInt(stat));

    if (!getline(*iss,stat,delim)) return;
    repairs->push_back(toInt(stat));
}

int StatsScreen::toInt(std::string str) {
    double vDouble = toDouble(str);
    return static_cast<int>(vDouble);
}

double StatsScreen::toDouble(std::string str) {
    std::istringstream convert(str);
    double vDouble;
    convert >> vDouble;
    return vDouble;
}

std::string StatsScreen::calcIndividualRating(CollaborationInfo *info) {
    // TODO: Calculate individual rating
    double rating = 0.0;
    PlayerStats *stats = info->getPlayerStats();

    double wallHitWeight, speedWeight,
        accuracyWeight, repairsWeight,
        destroyedWeight;

    int maxAvgSpeed, maxWallHits, maxDestroyed, maxRepairs;

    if (info->getGameRole() == PILOT) {
        //std::cout << "PILOT:\n";
        maxAvgSpeed = getMax(averageSpeeds);
        maxWallHits = getMax(wallHits);
        maxDestroyed = getMax(pilotDestroyed);
        maxRepairs = getMax(pilotRepairs);

        wallHitWeight = 0.3;
        speedWeight = 0.4;
        accuracyWeight = 0.1;
        destroyedWeight = 0.05;
        repairsWeight = 0.15;
    } else {
        //std::cout << "OTHER:\n";
        maxAvgSpeed = 0;
        maxWallHits = 0;
        maxDestroyed = getMax(nonPilotDestroyed);
        maxRepairs = getMax(nonPilotRepairs);

        wallHitWeight = 0.0;
        speedWeight = 0.0;
        destroyedWeight = 0.55;
        accuracyWeight = 0.15;
        repairsWeight = 0.3;
    }

    double accuracy;
    if (stats->shotsHit != 0) {
        accuracy = (double) stats->shotsHit / stats->shotsFired;
    } else {
        accuracy = 0.0;
    }

    double speedComp =  (maxAvgSpeed == 0.0) ?
                    0 : (maxRating*speedWeight) * (stats->averageSpeed / maxAvgSpeed);
    double collisionsComp = (maxWallHits == 0) ?
                    0 : (maxRating*wallHitWeight) * (1-(stats->numCollisions / maxWallHits));
    double destroyedComp = (maxDestroyed == 0) ?
                    0 : (maxRating*destroyedWeight) * (stats->enemiesDestroyed / (double) maxDestroyed);
    double repairsComp = (maxRepairs == 0) ?
                    0 : (maxRating*repairsWeight) * (stats->repairsMade / maxRepairs);
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

    int playersWeight = 0.7;
    int teamworkWeight = 0.3;

    int maxDamageSustained = getMax(damagesSustained);
    int maxGameLength = getMax(gameLengths);

    double damageComp = (maxDamageSustained == 0.0) ?
                    0 : (damageSustained / maxDamageSustained);

    double timeComp = (maxGameLength == 0) ?
                    0 : (gameLength / maxGameLength);

    int overall = ((pilotRating + engRating + navRating) / 3.0) * (maxRating * playersWeight)
                + (damageComp * timeComp) * (maxRating * teamworkWeight);

   //std::cout << ((pilotRating + engRating + navRating) / 3.0) << "\n";
   //std::cout << (damageComp * timeComp) * (maxRating * teamworkWeight) << "\n";

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
