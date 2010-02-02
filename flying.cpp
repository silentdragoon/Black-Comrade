#include "flying.h"
#include "const.h"

Flying::Flying(ShipControls *sc, CollisionManager *colMgr):
    colMgr(colMgr),
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
    position = new Vector3(1400.0, 0.0, 100.0 );
}

Flying::~Flying()
{}

void Flying::updateAngels()
{
    addPitch += (0.005*sc->forward());
    if( addPitch > 0.7 ) addPitch = 0.7;
    addRoll += (0.01*sc->side());
    if( addRoll > 1.0 ) addRoll = 1.0;
    flyYaw += (0.01*sc->yaw());
}

void Flying::updatePosition()
{
    Collision col = colMgr->shipMapCollision(position);
    if(col.isCollided)
    {
        for( int i = 0; i < 1; i += 3 )
        {
            xVel += col.normals[i];
        zVel += col.normals[i+2];
            cout << "n["<<i<<"] "<< col.normals[i] <<" "<< col.normals[i+1]
            <<" "<< col.normals[i+2] <<" "<<endl;
        }
    }
    
    
    //hack considering not all.
    double xzFor =  EngineForce*sin(addPitch);
    xVel += xzFor*sin(flyYaw);
    zVel += xzFor*cos(flyYaw);
    double xzSide = SideForce*sin(addRoll);
    xVel -= xzSide*sin(flyYaw+1.57079633);
    zVel -= xzSide*cos(flyYaw+1.57079633);

    xVel *= 0.94;
    zVel *= 0.94;
    
    addPitch *= 0.98;
    addRoll *= 0.98;
    
    position->x += xVel;
    position->z += zVel;
    
    pitch = addPitch;
    roll = addRoll;
    
    yaw = flyYaw;
    
    
    
    
    // double zcomp =   (forward*cos(pitch))*cos(yaw)+side*sin(yaw);
    // double xcomp =   (forward*cos(pitch))*sin(yaw)+side*cos(yaw);
    // double ycomp =   (forward*sin(pitch))+up*cos(pitch);
}

void Flying::tick()
{
    updateAngels();
    updatePosition();
}
