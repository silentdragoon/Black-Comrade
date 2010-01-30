#include "flying.h"
#include "const.h"

Flying::Flying(ShipControls *sc) :
    sc(sc),
    zVel(0.0),
    xVel(0.0),
    yVel(0.0),
    flyPitch(0.0),
    flyYaw(0.0),
    flyRoll(0.0),
    pitch(0.0),
    yaw(0.0),
    roll(0.0)
{
    position = new Vector3(0.0, 0.0, -500.0 );
}

Flying::~Flying()
{}

void Flying::updateAngels()
{
    addPitch += (0.1*sc->forward());
    if( addPitch > 1.0 ) addPitch = 1.0;
    addRoll += (0.1*sc->side());
    if( addRoll > 1.0 ) addRoll = 1.0;
    flyYaw += (0.1*sc->yaw());
}

void Flying::updatePosition()
{
    double xzF =  EngineForce*sin(addPitch);
    xVel += xzF*sin(flyYaw);
    zVel += xzF*cos(flyYaw);

    xVel *= 0.9;
    yVel *= 0.9;
    
    position->x += xVel;
    position->z += zVel;
    
    pitch = addPitch;
    roll = addRoll;
    
    
    
    
    // double zcomp =   (forward*cos(pitch))*cos(yaw)+side*sin(yaw);
    // double xcomp =   (forward*cos(pitch))*sin(yaw)+side*cos(yaw);
    // double ycomp =   (forward*sin(pitch))+up*cos(pitch);
}

void Flying::tick()
{
    //updateAngels();
    //updatePosition();
}
