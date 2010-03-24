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
    console->makeBlank();
    console->setString("========================================",0,1);
    console->setString("Repairbot Quick Task Helper v0.78 (beta)",0,2);
    console->setString("========================================",0,3);
    console->setString("",0,4);
    console->setString("Available commands:",0,5);
    console->setString("J: Clear tool jam",0,6);
    console->setString("R: Reboot bot",0,7);
    console->setString("O: Override safeties",0,8);
    console->setString("S: Self repair",0,9);
    console->setString("Q: Quit",0,10);
    console->setString("",0,11);
    console->setString("Enter to begin repairs...",0,12);
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

            gameTick = waitTicks;

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
                float perc = (float)gameTick/(float)waitTicks;
                broke=0;
                score = (int)abs(10*(perc+0.5));
                console->clearPrompt();
                std::stringstream out;
                out << perc*100.0;
                std::string result = "Repair efficiency: ";
                result+=out.str();
                result+="%";
                console->setString("                                             ",0,19);
                console->setString(result,0,19);
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
            console->makeBlank();
        }
    }

    updateProgressBar();

    if(inputState->isKeyDown(OIS::KC_Q)) isEnd=true;
}

void QuickTimeMiniGame::updateProgressBar() {
    if(begin) {
        float perc = (float)gameTick/(float)waitTicks*100.0;
        if(perc>100) perc = 100;

        for(int i=0;i<100;i++) {
            console->setChar('=',i,14);
            console->setChar('=',i,17);

            if(i<perc) {
                console->setChar('|',i,15);
                console->setChar('|',i,16);
            } else {
                console->setChar(' ',i,15);
                console->setChar(' ',i,16);
            }
        }
    }
    if(isEnd) {
        console->makeBlank();
    }
}

bool QuickTimeMiniGame::end() { return isEnd; }

void QuickTimeMiniGame::returnKeyPressed() {
    begin = true; 
}

int QuickTimeMiniGame::getScore() { return score; }

QuickTimeMiniGame::~QuickTimeMiniGame() {}
