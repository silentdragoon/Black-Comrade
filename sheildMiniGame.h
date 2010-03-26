#ifndef SHEILD_MINIGAME_H
#define SHEILD_MINIGAME_H

#include "IMiniGame.h"
#include "shipSystem.h"
#include "console.h"
#include "inputState.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>

#include "constManager.h"

#include <math.h>

using namespace std;

class SheildMiniGame : public IMiniGame
{
    private:
        bool isEnd;

        ShipSystem system;
        Console *console;
        InputState *inputState;
        
        std::vector<std::string> keys;
        std::string currentChoird;
        
        bool winLine;
        bool loseLine;
        
        int boardX, boardY;
        int boardHeight;
        int boardWidth;
        
        int heal;
        
        float dTime;
        int currentQ;
        float currentTime;
        
        void drawBoard();
        void drawLine(int index, std::string chars);
        void drawKeyStates();
        
        void loadFile(char *fileName);
        
    public:
        void tick();
        bool end();
        virtual bool complete();
        int getScore();

        ShipSystem getSystem();
        string getName();

        void alphaNumKeyPressed(const OIS::KeyEvent &arg);
        void returnKeyPressed();
        void backspaceKeyPressed();
        void otherKeyPressed(const OIS::KeyEvent &arg);

        SheildMiniGame(Console *console, InputState *inputState, int level);
        ~SheildMiniGame();
};

#endif
