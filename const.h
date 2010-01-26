
#ifndef CONST_H
#define CONST_H

class Const
{
public:

	// TICK_PERIOD
	static const double TICK_PERIOD = 0.01;

	// Constants for acceleration
    static const double TICKSFORWARD = 400;
    static const double TICKSSIDE = 100;
    static const double TICKSUP = 50;
    static const double TICKSYAW = 40;
    static const double TICKSPITCH = 20;
    
    // Constants for motion
    static const double FORWARD_SPEED= 2;
    static const double SIDE_SPEED = 1;
    static const double TURN_SPEED = 0.0175;
    static const double UP_SPEED = 0.5;
    
    static const double MAX_BANK = 0.2;
    
};

#endif
