#include "flying.h"
#include "const.h"

Flying::Flying( SceneNodeManager *snMgr, PilotControls *sc, ShipState *shipState,
                DamageState *damageState, CollisionManager *colMgr,
                SystemManager *systemManager, bool useCollisions,
                double iXPos, double iYPos, double iZPos, double iYaw,
                PlayerStats *pilotStats):
    snMgr(snMgr),
    colMgr(colMgr),
    sc(sc),
    shipState(shipState),
    damageState(damageState),
    systemManager(systemManager),
    useCollisions(useCollisions),
    zVel(0.0),
    xVel(0.0),
    yVel(0.0),
    flyPitch(0.0),
    flyYaw(iYaw),
    flyRoll(0.0),
    pitch(0.0),
    yaw(0.0),
    roll(0.0),
    hitCountDown(0),
    addRoll(0.0),
    addPitch(0.0),
    addYaw(0.0),
    yawMom(0.0),
    lastAverage(0),
    averageDelay(150),
    averageSpeed(0.0),
    numSpeedsTaken(0),
    pilotStats(pilotStats)
{
    position = new Vector3(iXPos, iYPos, iZPos );
}

Flying::~Flying()
{}

void Flying::updateAngels()
{
    if( hitCountDown == 0)
    {
        flyPitch += (0.004*sc->forward());
        if( flyPitch > 0.4 ) flyPitch = 0.4;
        flyRoll += (0.01*sc->side());
        if( flyRoll > 1.0 ) flyRoll = 1.0;
        yawMom += (0.0015*sc->yaw());
    }
}

void Flying::updatePosition()
{
    bool hitObj = false;
    bool collided = false;
    Collision col = colMgr->collideWithMapPieceAndMovableObjects(snMgr->getEntity(shipState));
    if (col.isCollided) {
        // Collided with map
        collided = true;
    } else {
        collided = colMgr->collideEntityWithObj(snMgr->getEntity(shipState));
        hitObj = collided;
    }

    if(collided && useCollisions)
    {
        vFactor = 0.05;
        
        //reflected vel need to be fixed
        
        float pen = col.penetration[0];
        Vector3 normal(col.normals[0],col.normals[1],col.normals[2]);
        Vector3 vel(xVel, yVel, zVel);
        
        *position += normal * (pen + 1);
        
		float speedIn = vel.dotProduct(-normal) / normal.length();
        speedIn = abs(speedIn);
		if(speedIn > 0) {
			hitCountDown = static_cast<int> (50 * speedIn);
		
			vel = vel + (2 * (normal.dotProduct(-vel)) * normal);

			vel.normalise();
		
			vel *= 0.95 * speedIn;

			xVel = vel.x;
			yVel = vel.y;
			zVel = vel.z;
		}

        if (hitObj) {
            damageState->damage(100);
        } else {
            damageState->damage(10);
        }
        pilotStats->numCollisions ++;
    }

    if( hitCountDown == 0 )
    {
        updateAngels();
        //hack considering not all. Works fine though
        if( damageState->getEngineHealth() > 0 )
        {
            double engineRate = systemManager->getEngineRate();
            double xzFor =  0.7*(engineRate/1.3+0.4)*EngineForce*sin(flyPitch);
            xVel += xzFor*sin(flyYaw);
            zVel += xzFor*cos(flyYaw);
            double xzSide = (engineRate+0.1)*SideForce*sin(flyRoll);
            xVel -= xzSide*sin(flyYaw+1.57079633);
            zVel -= xzSide*cos(flyYaw+1.57079633);
    
            yVel += 0.025* sc->up();
        }
    
        addRoll = 0.0;
        addYaw = 0.0;
        addPitch = 0.0;
    }
    else
    {
        double r = 1.0 *rand()/RAND_MAX-0.5;
        addRoll = r * vFactor;
        addYaw = r * vFactor;
        addPitch = r * vFactor;
        vFactor *= 0.93;
        hitCountDown--;
    }

    xVel *= 0.96;
    zVel *= 0.96;
    yVel *= 0.9;

    flyPitch *= 0.98;
    flyRoll *= 0.98;
    yawMom *= 0.9;

    flyYaw += yawMom;

    position->x += xVel;
    position->z += zVel;
    position->y += yVel;

    pitch = flyPitch + addPitch;
    roll = flyRoll + addRoll;

    yaw = flyYaw + addYaw;
}

void Flying::updateShipState()
{
    shipState->setX(position->x);
    shipState->setY(position->y);
    shipState->setZ(position->z);

    shipState->yaw = yaw;
    shipState->pitch = -pitch;
    shipState->roll = -roll;
}

void Flying::updateAverageSpeed() {
    numSpeedsTaken ++;
    averageSpeed = averageSpeed * ((numSpeedsTaken-1.0)/numSpeedsTaken) + getSpeed() * (1.0/numSpeedsTaken);
    pilotStats->averageSpeed = averageSpeed;
}

void Flying::tick()
{
    shipState->setSpeed(getSpeed());
    updatePosition();
    updateShipState();
    if (lastAverage > averageDelay) {
        updateAverageSpeed();
        lastAverage = 0;
    } else {
        lastAverage ++;
    }
}

double Flying::getSpeed()
{
    return sqrt( xVel*xVel+zVel*zVel) * 3.6 * 60;
}
