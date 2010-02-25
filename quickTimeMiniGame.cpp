#include "quickTimeMiniGame.h"

QuickTimeMiniGame::QuickTimeMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , score(0)
    , ticks(0)
    , begin(false)
{
    console->setVisible(true);
    console->append("========================================");
    console->append("Repairbot Quick Task Helper v0.78 (beta)");
    console->append("========================================");
    console->append("");
    console->append("Available commands:");
    console->append("j: Clear tool jam");
    console->append("r: Reboot bot");
    console->append("o: Override safeties");
    console->append("s: Self repair");
    console->append("");
    console->append("Enter to begin repairs...");
    saveTick = 0;
}

void QuickTimeMiniGame::tick() {
    if(begin) {
        // Gameplay here
        ticks++;
        if(ticks==1) console->append("Repairing...");

        if((ticks-saveTick)==120) {

            saveTick = ticks;

            // Randomly play game
            srand(time(NULL));
            int irand = rand()%6+1;

            switch(irand) {
                case 1:
                    console->appendToPrompt("Jammed repair tool...");
                    broke=1;
                    gameTick=90;
                    break;
                case 2:
                    console->appendToPrompt("Reboot, operating system crash...");
                    broke=2;
                    gameTick=90;
                    break;
                case 3:
                    console->appendToPrompt("Override to overcome safety limits...");
                    broke=3;
                    gameTick=90;
                    break;
                case 4:
                    console->appendToPrompt("Self repair bot, damage sustained...");
                    broke=4;
                    gameTick=90;
                    break;
                case 5:
                    console->appendToPrompt("OK...");
                    broke=0;
                    // Lucky!
                    break;
                case 6:
                    console->appendToPrompt("OK...");
                    broke=0;
                    // Lucky!
                    break;
            }        
        }

        if(broke!=0) {
            win=false;
            if(gameTick==0) {
                broke=0;
                console->clearPrompt();
            }

            if(inputState->isKeyDown(OIS::KC_J)&&(broke==1)) win=true;
            if(inputState->isKeyDown(OIS::KC_R)&&(broke==2)) win=true;
            if(inputState->isKeyDown(OIS::KC_O)&&(broke==3)) win=true;
            if(inputState->isKeyDown(OIS::KC_S)&&(broke==4)) win=true;
            if(win) {
                broke=0;
                score++;
                console->clearPrompt();
            }
            gameTick--;
        }
        

        if(ticks>1320) {
            isEnd = true;
            console->setVisible(false);
        }
    }

    if(inputState->isKeyDown(OIS::KC_RETURN)) {
        begin = true; 
    }

    if(inputState->isKeyDown(OIS::KC_F2)) {
        isEnd = true;
        console->append("");
        console->setVisible(false);
    }
}

bool QuickTimeMiniGame::end() { return isEnd; }

int QuickTimeMiniGame::getScore() { return score; }

QuickTimeMiniGame::~QuickTimeMiniGame() {}
