#include "consoleShell.h"

ConsoleShell::ConsoleShell(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , command("")
    , defaultPrompt(">> ")
    , gameToPlay(NULL)
{
    console->appendLine("---------------------------------------");
    console->appendLine("BlackComrade Ship System v0.5 (beta)");
    console->appendLine("---------------------------------------");
    console->appendLine("Type 'help' for a list of available commands");
    showPrompt();
}

void ConsoleShell:: tick() {
    gameToPlay = NULL;
}

IMiniGame* ConsoleShell::getGameToPlay() { return gameToPlay; }

void ConsoleShell::processCommand() {
    boost::algorithm::trim(command);
    if (command == "") return;
    
    if (command == "help") {
        console->appendLine("repair");
    } else if (command == "repair" ) {
        gameToPlay = new QuickTimeMiniGame(console,inputState);
    } else {
        command += ": command not found";
        console->appendLine(command);
    }
    command = "";
}

void ConsoleShell::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
    command += arg.text;
    console->appendToPrompt(arg.text);
}

void ConsoleShell::backspaceKeyPressed () {
    if (command.length() == 0) return;
    command = command.substr(0,command.length() -1);
    console->backSpacePrompt();
}

void ConsoleShell::returnKeyPressed() {
    std::string toAdd = defaultPrompt;
    toAdd += command;
    console->appendLine(toAdd);
    console->clearPrompt();
    console->appendToPrompt(defaultPrompt);
    processCommand();
}

void ConsoleShell::showPrompt() {
    console->clearPrompt();
    console->appendToPrompt(defaultPrompt);
}

ConsoleShell::~ConsoleShell() {}
