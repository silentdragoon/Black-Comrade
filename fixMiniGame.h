#ifndef FIX_MINIGAME_H
#define FIX_MINIGAME_H

#include "IMiniGame.h"
#include "shipSystem.h"

class FixMiniGame : public IMiniGame
{
    private:
        bool isEnd;

        ShipSystem system;
        
    public:
        void tick();
        bool end();
        int getScore();

        ShipSystem getSystem();

        void returnKeyPressed();

        FixMiniGame();
        ~FixMiniGame();
};

#endif
