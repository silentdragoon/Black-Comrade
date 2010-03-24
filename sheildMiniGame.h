#ifndef SHEILD_MINIGAME_H
#define SHEILD_MINIGAME_H

#include "IMiniGame.h"
#include "shipSystem.h"
#include "console.h"

class SheildMiniGame : public IMiniGame
{
    private:
        bool isEnd;

        ShipSystem system;
        Console *console;
        
    public:
        void tick();
        bool end();
        int getScore();

        ShipSystem getSystem();

        void alphaNumKeyPressed(const OIS::KeyEvent &arg);
        void returnKeyPressed();
        void backspaceKeyPressed();
        void otherKeyPressed(const OIS::KeyEvent &arg);

        SheildMiniGame(Console *console);
        ~SheildMiniGame();
};

#endif
