#include "consoleMiniGame.h"

ConsoleMiniGame::ConsoleMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , command("")
    , defaultPrompt(">> ")
    , score(0)
{
    console->appendLine("---------------------------------------");
    console->appendLine("BlackComrade Ship System v0.5 (beta)");
    console->appendLine("---------------------------------------");
    console->appendLine("Type 'help' for a list of available commands");
    console->appendToPrompt(defaultPrompt);
}

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->appendLine("Score");
        score = score + 1;
    }
}

bool ConsoleMiniGame::end() { return isEnd; }

int ConsoleMiniGame::getScore() {
    return score;
}

void ConsoleMiniGame::processCommand() {
    if (command == "help") {
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

void ConsoleMiniGame::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
    command += arg.text;
    console->appendToPrompt(arg.text);
}

void ConsoleMiniGame::backspaceKeyPressed () {
    if (command.length() == 0) return;
    command = command.substr(0,command.length() -1);
    console->backSpacePrompt();
}

void ConsoleMiniGame::returnKeyPressed() {
    boost::algorithm::trim(command);
    if (command == "") return;
    console->clearPrompt();
    std::string toAdd = defaultPrompt;
    toAdd += command;
    console->appendLine(toAdd);
    processCommand();
    console->appendToPrompt(defaultPrompt);
}

ConsoleMiniGame::~ConsoleMiniGame() {}
