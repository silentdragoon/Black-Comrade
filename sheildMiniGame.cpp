
#include "sheildMiniGame.h"

SheildMiniGame::SheildMiniGame(Console *console)
    : console(console)
    , isEnd(false)
{
    console->appendToPrompt("Yeah Baby");
}

void SheildMiniGame::tick()
{

}

bool SheildMiniGame::end()
{
    return isEnd;
}

int SheildMiniGame::getScore()
{

}

ShipSystem SheildMiniGame::getSystem()
{

}

void SheildMiniGame::alphaNumKeyPressed(const OIS::KeyEvent &arg) 
{
    console->appendToPrompt((char)arg.text);
}

void SheildMiniGame::returnKeyPressed() 
{
    isEnd = true;
}

void SheildMiniGame::backspaceKeyPressed() 
{

}

void SheildMiniGame::otherKeyPressed(const OIS::KeyEvent &arg)
{

}

        
