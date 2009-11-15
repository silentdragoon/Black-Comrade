#ifndef I_INPUT_STATE_H
#define I_INPUT_STATE_H

class IInput
{
  public:
    virtual double forwardInput() = 0;
    virtual double sideInput() = 0;
    virtual double upInput() = 0;
    virtual double yawInput() = 0;
    virtual double pitchInput() = 0;
};

#endif
