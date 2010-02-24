#include "consoleMiniGame.h"

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->append("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_2)) {
        inputState->clearKeyListener();
        isEnd = true;
        console->append("");
        console->setVisible(false);
    }
}

bool ConsoleMiniGame::end() { return isEnd; }

int ConsoleMiniGame::getScore() {
    return score;
}
   
ConsoleMiniGame::ConsoleMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , score(0)
{
    inputState->addKeyListener(this);
    console->setVisible(true);
    console->append("---------------------------------------");
    console->append("BlackComrade System Repair v0.5 (beta)");
    console->append("---------------------------------------");
}

bool ConsoleMiniGame::keyPressed (const OIS::KeyEvent &arg) {
    
    if (arg.text == 13) {
        console->enterCommand();
        return true;
    } else if (arg.key == OIS::KC_BACK) {
        console->backSpace();
        return true;
    }

    char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
    for(int c=0;c<sizeof(legalchars);c++){
       if(legalchars[c]==arg.text){
          console->typeShit(arg.text);
          break;
        }
    }
    return true;
}

bool ConsoleMiniGame::keyReleased (const OIS::KeyEvent &arg) {
    return false;
}

ConsoleMiniGame::~ConsoleMiniGame() {}
