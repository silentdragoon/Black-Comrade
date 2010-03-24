#include "consoleShell.h"

ConsoleShell::ConsoleShell(Console *console, InputState *inputState, IExit *exit)
    : console(console)
    , inputState(inputState)
    , command("")
    , commandIndex(-1)
    , defaultPrompt(">> ")
    , gameToPlay(NULL)
    , exit(exit)
    , fullCommand("")
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
    fullCommand = "";
}

std::string ConsoleShell::getCommand()
{
    return fullCommand;
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
        console->appendLine(" repair SYSNAME        Launches System Repair for the specified system");
        console->appendLine(" help                  Shows available commands");
        console->appendLine(" reboot                Reboot the ship's systems (may cause unpredictable results)");
    } else if (command == "repair" ) {
        console->appendLine("Error: No target system specified.");
        console->appendLine("Usage: repair [shieldgen | weapons | sensors | engines]");
    } else if (command == "repair shieldgen" ) {
        gameToPlay = new QuickTimeMiniGame(console,inputState,SS_SHIELD_GENERATOR);
    } else if (command == "repair weapons" ) {
        gameToPlay = new ButtonMashMiniGame(console,inputState);
    } else if (command == "repair sensors" ) {
        gameToPlay = new QuickTimeMiniGame(console,inputState,SS_SENSORS);
    } else if (command == "repair engines" ) {
        gameToPlay = new QuickTimeMiniGame(console,inputState,SS_ENGINES);
    } else if (command == "access main program" ) {
        console->appendLine("access: PERMISSION DENIED.");
    } else if (command == "access main program grid" ) {
        console->appendLine("access: PERMISSION DENIED. and...");
        gameToPlay = new MagicWordMiniGame(console,inputState);
    } else if (command == "access main security" ) {
        console->appendLine("access: PERMISSION DENIED.");
    } else if (command == "reboot") {
        console->appendLine("Rebooting system...");
        exit->exit();
    } else if (command == "test") {
        gameToPlay = new TestMiniGame(console,inputState);
    } else if (command == "fix") {
        console->appendLine("Fixing all Systems...Fixed!");
        gameToPlay = new FixMiniGame();
    } else {
        command += ": command not found";
        console->appendLine(command);
    }
    command = "";
    commandIndex = commands.size() - 1;
}

void ConsoleShell::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
    if (command.length() > (CONSOLE_LENGTH - defaultPrompt.length() - 1)) return;

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
