#include "quickTimeMiniGame.h"

QuickTimeMiniGame::QuickTimeMiniGame(Console *console, InputState *inputState,
                                     ShipSystem system)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , score(0)
    , ticks(0)
    , begin(false)
    , system(system)
{
    console->appendLine("========================================");
    console->appendLine("Repairbot Quick Task Helper v0.78 (beta)");
    console->appendLine("========================================");
    console->appendLine("");
    console->appendLine("Available commands:");
    console->appendLine("j: Clear tool jam");
    console->appendLine("r: Reboot bot");
    console->appendLine("o: Override safeties");
    console->appendLine("s: Self repair");
    console->appendLine("q: Quit");
    console->appendLine("");
    console->appendLine("Enter to begin repairs...");
    saveTick = 0;

    endTicks = (int)ceil(20.0 / ConstManager::getFloat("tick_period"));
    periodTicks = abs(endTicks / 11);
    waitTicks = periodTicks - 40;
}

ShipSystem QuickTimeMiniGame::getSystem() {
    return system;
}

void QuickTimeMiniGame::tick() {
    if(begin) {
        score = 0;
        // Gameplay here
        ticks++;
        if(ticks==1) console->appendLine("Repairing...");

        if((ticks-saveTick)==periodTicks) {

            saveTick = ticks;

            // Randomly play game
            srand(time(NULL));
            int irand = rand()%4+1;

            gameTick = periodTicks;

            switch(irand) {
                case 1:
                    console->clearPrompt();
                    console->appendToPrompt("Jammed repair tool...");
                    broke=1;
                    break;
                case 2:
                    console->clearPrompt();
                    console->appendToPrompt("Reboot, operating system crash...");
                    broke=2;
                    break;
                case 3:
                    console->clearPrompt();
                    console->appendToPrompt("Override to overcome safety limits...");
                    broke=3;
                    break;
                case 4:
                    console->clearPrompt();
                    console->appendToPrompt("Self repair bot, damage sustained...");
                    broke=4;
                    break;
            }        
        }

        if(broke!=0) {
            win=false;
            loose=false;
            if(gameTick<0) {
                broke=0;
                console->clearPrompt();
            }

            if(broke==1) {
                if(inputState->isKeyDown(OIS::KC_J)) win=true;
                if(inputState->isKeyDown(OIS::KC_R)) loose=true;
                if(inputState->isKeyDown(OIS::KC_O)) loose=true;
                if(inputState->isKeyDown(OIS::KC_S)) loose=true;
            } else if(broke==2) {
                if(inputState->isKeyDown(OIS::KC_J)) loose=true;
                if(inputState->isKeyDown(OIS::KC_R)) win=true;
                if(inputState->isKeyDown(OIS::KC_O)) loose=true;
                if(inputState->isKeyDown(OIS::KC_S)) loose=true;
            } else if(broke==3) {
                if(inputState->isKeyDown(OIS::KC_J)) loose=true;
                if(inputState->isKeyDown(OIS::KC_R)) loose=true;
                if(inputState->isKeyDown(OIS::KC_O)) win=true;
                if(inputState->isKeyDown(OIS::KC_S)) loose=true;
            } else if(broke==4) {
                if(inputState->isKeyDown(OIS::KC_J)) loose=true;
                if(inputState->isKeyDown(OIS::KC_R)) loose=true;
                if(inputState->isKeyDown(OIS::KC_O)) loose=true;
                if(inputState->isKeyDown(OIS::KC_S)) win=true;
            } else {
                // NOTHING
            }

            if(win) {
                broke=0;
                score = 10;
                console->clearPrompt();
                console->appendToPrompt("OK");
            }

            if(loose) {
                broke=0;
                console->clearPrompt();
                console->appendToPrompt("ERROR");
            }

            gameTick--;
        }
        

        if((float)ticks>endTicks) {
            isEnd = true;
        }
    }
    if(inputState->isKeyDown(OIS::KC_Q)) isEnd=true;
}

bool QuickTimeMiniGame::end() { return isEnd; }

void QuickTimeMiniGame::returnKeyPressed() {
    begin = true; 
}

int QuickTimeMiniGame::getScore() { return score; }

QuickTimeMiniGame::~QuickTimeMiniGame() {}
