
#ifndef CONST_H
#define CONST_H

class Const
{
public:

	// TICK_PERIOD
	#define TICK_PERIOD 0.01

	// TODO: This does not work correctly, twice the clock period and
	// twice the TICKSFORWARD does not result in the same movement :(

	// Constants for acceleration
    static const double TICKSFORWARD = 40000.0 * TICK_PERIOD;
    static const double TICKSSIDE = 10000.0 * TICK_PERIOD;
    static const double TICKSUP = 5000.0 * TICK_PERIOD;
    static const double TICKSYAW = 4000.0 * TICK_PERIOD;
    static const double TICKSPITCH = 2000.0 * TICK_PERIOD;
    
    // Constants for motion
    static const double FORWARD_SPEED= 200.0 * TICK_PERIOD;
    static const double SIDE_SPEED = 100.0 * TICK_PERIOD;
    static const double TURN_SPEED = 1.75 * TICK_PERIOD;
    static const double UP_SPEED = 50.0 * TICK_PERIOD;
    
    static const double MAX_BANK = 20.0 * TICK_PERIOD;
    
    // Constants for front gun
    static const double MIN_SHOOT_PERIOD = 800.0 * TICK_PERIOD;
    
    // Constants for enemy behav.
    static const float FVELOCITY = 225.0 * TICK_PERIOD;
    //lookahead in ticks
    static const float LOOKA = 300.0 * TICK_PERIOD;
    
    // Constants for network
    static const int SERVER_PORT=6005;
};

#endif
