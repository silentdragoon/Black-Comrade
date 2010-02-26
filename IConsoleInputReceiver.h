#ifndef CONSOLE_INPUT_RECEIVER_H
#define CONSOLE_INPUT_RECEIVER_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

class IConsoleInputReceiver {
public:
    virtual void alphaNumKeyPressed(const OIS::KeyEvent &arg) {};
    virtual void returnKeyPressed() {};
    virtual void backspaceKeyPressed() {};
    virtual void otherKeyPressed(const OIS::KeyEvent &arg) {};
};

#endif
