#include "quickTimeMiniGame.h"

QuickTimeMiniGame::QuickTimeMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , score(0)
    , ticks(0)
    , begin(false)
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
}

ShipSystem QuickTimeMiniGame::getSystem() {
    return SS_SHIELD_GENERATOR;
}

void QuickTimeMiniGame::tick() {
    if(begin) {
        // Gameplay here
        ticks++;
        if(ticks==1) console->appendLine("Repairing...");

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
