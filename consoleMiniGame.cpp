#include "consoleMiniGame.h"

ConsoleMiniGame::ConsoleMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , command("")
    , score(0)
{
    inputState->addKeyListener(this);
    console->setVisible(true);
    console->append("---------------------------------------");
    console->append("BlackComrade System Repair v0.5 (beta)");
    console->append("---------------------------------------");
}

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->append("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_F2)) {
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

void ConsoleMiniGame::processCommand() {
    boost::algorithm::trim(command);
    if (command == "") return;
    if (command == "dir" || command == "ls") {
        console->append("repair");
    } else if (command == "cd repair") {
    } else if (command == "sudo repairbot activate") {
        console->append("**** Activating repairbot1 ****");
    } else {
        command += ": command not found";
        console->append(command);
    }
    command = "";
}

bool ConsoleMiniGame::keyPressed (const OIS::KeyEvent &arg) {

    if (arg.key == OIS::KC_RETURN) {
        console->enterCommand();
        processCommand();
        return true;
    } else if (arg.key == OIS::KC_BACK) {
        console->backSpace();
        if(command.size()>0) {
            command=command.substr(0,command.length()-1);
        }
        return true;
    } else if (arg.text == 0) {
        return true;
    }

    char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
    for(int c=0;c<sizeof(legalchars);c++){
        if(legalchars[c]==arg.text){
            console->typeShit(arg.text);
            command += arg.text;
            break;
        }
    }
    return true;
}

bool ConsoleMiniGame::keyReleased (const OIS::KeyEvent &arg) {
    return false;
}

ConsoleMiniGame::~ConsoleMiniGame() {}
