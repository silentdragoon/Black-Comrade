#include "consoleMiniGame.h"

ConsoleMiniGame::ConsoleMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , command("")
    , score(0)
{
    console->setVisible(true);
    console->appendLine("---------------------------------------");
    console->appendLine("BlackComrade System Repair v0.5 (beta)");
    console->appendLine("---------------------------------------");
}

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->appendLine("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_F2)) {
        isEnd = true;
        console->appendLine("");
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
        console->appendLine("repair");
    } else if (command == "cd repair") {
    } else if (command == "sudo repairbot activate") {
        console->appendLine("**** Activating repairbot1 ****");
    } else {
        command += ": command not found";
        console->appendLine(command);
    }
    command = "";
}

void ConsoleMiniGame::keyPressed (const OIS::KeyEvent &arg) {

    if (arg.key == OIS::KC_RETURN) {
        console->returnKeyPrompt();
        processCommand();
        return;
    } else if (arg.key == OIS::KC_BACK) {
        console->backSpacePrompt();
        if(command.size()>0) {
            command=command.substr(0,command.length()-1);
        }
        return;
    } else if (arg.text == 0) {
        return;
    }

    char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
    for(int c=0;c<sizeof(legalchars);c++){
        if(legalchars[c]==arg.text){
            console->appendToPrompt(arg.text);
            command += arg.text;
            break;
        }
    }
}

ConsoleMiniGame::~ConsoleMiniGame() {}
