#include "consoleShell.h"

ConsoleShell::ConsoleShell(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , command("")
    , commandIndex(-1)
    , defaultPrompt(">> ")
    , gameToPlay(NULL)
{
    commands = std::vector<std::string>();
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
    if (command == "") {
        commandIndex = commands.size();
        return;
    }
    
    commands.push_back(command);
    if (command == "help") {
        console->appendLine("Available commands:");
        console->appendLine(" repair                Launches BlackComrade System Repair");
        console->appendLine(" help                  Shows available commands");
    } else if (command == "repair" ) {
        gameToPlay = new QuickTimeMiniGame(console,inputState);
    } else {
        command += ": command not found";
        console->appendLine(command);
    }
    command = "";
    commandIndex = commands.size() - 1;
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

void ConsoleShell::otherKeyPressed (const OIS::KeyEvent &arg) {
    if (arg.key == OIS::KC_UP && commands.size() > 0) {
        historyBack();
    } else if (arg.key == OIS::KC_DOWN) {
        historyForward();
    }
    std::cout << commandIndex << std::endl;
}

void ConsoleShell::historyBack() {
    if (commandIndex < 0) return;
    showCommand(commandIndex);
    if (commandIndex > 0) commandIndex -= 1;
}

void ConsoleShell::historyForward() {
    int numCommands = commands.size();
    if (commandIndex + 1 > numCommands - 1) {
        showPrompt();
    } else {
        showCommand(commandIndex + 1);
        commandIndex += 1;
    }
}

void ConsoleShell::showCommand(int index) {
    if (index < 0 || index > commands.size() - 1) return;
    showPrompt();
    command = commands.at(index);
    console->appendToPrompt(commands.at(index));
}

void ConsoleShell::showPrompt() {
    command = "";
    console->clearPrompt();
    console->appendToPrompt(defaultPrompt);
}

ConsoleShell::~ConsoleShell() {}
