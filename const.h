
#ifndef CONST_H
#define CONST_H

class Const
{
public:

	#define PI 3.14159265

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
    static const double MIN_SHOOT_PERIOD = 1800.0 * TICK_PERIOD;
    static const int FRONT_BULLET_SPEED = 50;
    
    // Constants for enemy behav.
    static const float ENEMY_PATROL_SPEED = 50.0 * TICK_PERIOD;
    static const float ENEMY_ATTACK_SPEED = 100.0 * TICK_PERIOD;
    // How many times its verlocity you look ahead
    static const float LOOKA = 15;
    static const float ENEMY_SIGHT_DIS = 150;
    static const float ENEMY_SIGHT_ANGLE = 0.5236;
    
    static const float TURN_TO_LOOK_STEP = 0.01;
    static const float SWARM_TARGET_DIS = 35;
    
    // Constants for network
    static const int SERVER_PORT=6005;

    // Constants for swarm managing
    static const double SPAWN_DELAY=1.0 / TICK_PERIOD;

    // Constants for bulletManager

    // Constants for mapManager
    static const int MAPSIZE = 30;
    static const int TILE_SIZE = 400;
};

#endif
