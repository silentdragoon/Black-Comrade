#ifndef CONSOLE_INPUT_RECEIVER_H
#define CONSOLE_INPUT_RECEIVER_H

class IConsoleInputReceiver {
public:
    virtual void alphaNumKeyPressed(const OIS::KeyEvent &arg) {};
    virtual void returnKeyPressed() {};
    virtual void backspaceKeyPressed() {};
};

#endif
