#ifndef CONSOLE_SHELL_H
#define CONSOLE_SHELL_H

#include <iostream>
#include <string>
#include <vector>
#include "IConsoleInputReceiver.h"
#include "IMiniGame.h"
#include "ITickable.h"
#include "console.h"
#include "inputState.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <boost/algorithm/string/trim.hpp>

#include "quickTimeMiniGame.h"
#include "magicWordMiniGame.h"
#include "buttonMashMiniGame.h"

class ConsoleShell : public IConsoleInputReceiver
{
    private:
        InputState *inputState;
        Console *console;

        IMiniGame *gameToPlay;

        std::string defaultPrompt;
        std::string command;
        std::vector<std::string> commands;
        int commandIndex;

        void historyBack();
        void historyForward();
        void showCommand(int index);

        void processCommand();
        
    public:
        void tick();
        IMiniGame* getGameToPlay();

        void alphaNumKeyPressed (const OIS::KeyEvent &arg);
        void backspaceKeyPressed();
        void returnKeyPressed();
        void otherKeyPressed (const OIS::KeyEvent &arg);

        void showPrompt();
    
        ConsoleShell(Console *console, InputState *inputState);
        ~ConsoleShell();
};

#endif
