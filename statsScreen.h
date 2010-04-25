#ifndef STATS_SCREEN_H
#define STATS_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include "collaborationInfo.h"
#include "gameStateMachine.h"

#include <fstream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

class StatsScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        CollaborationInfo *pilotInfo;
        CollaborationInfo *navInfo;
        CollaborationInfo *engInfo;

        int numberOfSavedGames;        

        std::vector<double> averageSpeeds;
        std::vector<int> wallHits;
        std::vector<int> pilotShots;
        std::vector<int> nonPilotShots;
        std::vector<int> pilotHits;
        std::vector<int> nonPilotHits;
        std::vector<int> pilotDestroyed;
        std::vector<int> nonPilotDestroyed;
        std::vector<int> pilotRepairs;
        std::vector<int> nonPilotRepairs;

        char delim;

        GameState finishState;

        void loadExistingStats(std::string fileName);
        void parseCommonStats(std::istringstream *iss, bool pilot);

        double getAverage(std::vector<int> stats);
        double getAverage(std::vector<double> stats);

        double getMax(std::vector<int> stats);
        double getMax(std::vector<double> stats);

        void saveStats(std::string fileName);

        int toInt(std::string str);
        double toDouble(std::string str);

        void addStats(CollaborationInfo *info, int columnOffset);
        void addOverallRating();

        std::string calcIndividualRating(CollaborationInfo *info);
        std::string calcOverallRating();
        
        float wpx;
        float hpx;
        
        int maxRating;

    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        MenuType nextMenu();

        StatsScreen(InputState *inputState, GuiManager *guiManager,
                    CollaborationInfo *pilotInfo,
                    CollaborationInfo *navInfo,
                    CollaborationInfo *engInfo,
                    GameState finishState);

        ~StatsScreen();
};

#endif
