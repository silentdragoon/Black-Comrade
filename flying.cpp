#include "flying.h"
#include "const.h"

Flying::Flying(KeyState *ks,Vector3 *position,Vector3 *orientation) :
    ks(ks),
    position(position),
    orientation(orientation)
{
    velocity = new Vector3(0.0,0.0,0.0);
    angularVelocity = new Vector3(0.0,0.0,0.0);
}

Flying::~Flying()
{}

double Flying::getDrag(string dir)
{
    double v2 = 0.0;
    if(dir=="x") v2 = velocity->x;
    if(dir=="y") v2 = velocity->y;
    if(dir=="z") v2 = velocity->z;

    int m = 1;
    if(v2<0) m = -1;

    v2 = v2*v2; // Square the velocity for a laugh
    //cout << "v2: " << v2 << endl;

    double drag = m*0.5*Const::AIR_DENSITY*v2*Const::DRAG_COEFFICIENT*Const::REFERENCE_AREA;
    //cout << "Drag: " << drag << endl;
    return drag;
}

double Flying::getVelocity()
{
    double xvel = velocity->x;
    double yvel = velocity->y;
    double zvel = velocity->z;

    return sqrt((xvel*xvel)+(yvel*yvel)+(zvel*zvel));
}

void Flying::updatePosition()
{
    orientation->x=orientation->x+angularVelocity->x;
    orientation->y=orientation->y+angularVelocity->y;
    orientation->z=orientation->z+angularVelocity->z;

    position->x=position->x+0;
    position->y=position->y+(velocity->y*cos(orientation->y))+(velocity->z*sin(orientation->x));
    position->z=position->z+(velocity->z*cos(orientation->x)*cos(orientation->y))+(velocity->x*cos(orientation->y));

    //cout << position->x << " " << position->y << " " << position->z << endl;


    cout << orientation->x << " " << orientation->y << " " << orientation->z << endl;
}

void Flying::changeVector()
{

}

void Flying::tick()
{

    bool keypress=false;

    if(ks->isKeyDown(OIS::KC_A)) {
        double accel = (Const::SHIP_SIDE_THRUST-getDrag("x"))/Const::SHIP_MASS;
        velocity->x = velocity->x + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_D)) {
        double accel = ((-1.0*(Const::SHIP_SIDE_THRUST))-getDrag("x"))/Const::SHIP_MASS;
        velocity->x = velocity->x + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_SPACE)) {
        double accel = (Const::SHIP_UP_THRUST-getDrag("y"))/Const::SHIP_MASS;
        velocity->y = velocity->y + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_LSHIFT)) {
        double accel = ((-1.0*(Const::SHIP_UP_THRUST))-getDrag("y"))/Const::SHIP_MASS;
        velocity->y = velocity->y + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_W)) {
        double accel = (Const::SHIP_FORWARD_THRUST-getDrag("z"))/Const::SHIP_MASS;
        velocity->z = velocity->z + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_S)) {
        double accel = ((-1.0*(Const::SHIP_FORWARD_THRUST))-getDrag("z"))/Const::SHIP_MASS;
        velocity->z = velocity->z + accel;
        keypress=true;
    } 
    if(ks->isKeyDown(OIS::KC_DOWN)) {
        double accel = (-1.0*Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->x = angularVelocity->x + accel;
        keypress=true;
    }
    if(ks->isKeyDown(OIS::KC_UP)) {
        double accel = (Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->x = angularVelocity->x + accel;
        keypress=true;
    }
    if(ks->isKeyDown(OIS::KC_LEFT)) {
        double accel = (Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->y = angularVelocity->y + accel;
        keypress=true;
    }
    if(ks->isKeyDown(OIS::KC_RIGHT)) {
        double accel = (-1.0*Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->y = angularVelocity->y + accel;
        keypress=true;
    } 
    if(!keypress) {
        double accelx = -getDrag("x")/Const::SHIP_MASS;
        double accely = -getDrag("y")/Const::SHIP_MASS;
        double accelz = -getDrag("z")/Const::SHIP_MASS;

        velocity->x = velocity->x + accelx; // RELATIVE TO SHIP
        velocity->y = velocity->y + accely;
        velocity->z = velocity->z + accelz;

        angularVelocity->x = angularVelocity->x/1.1;
        angularVelocity->y = angularVelocity->y/1.1;
        angularVelocity->z = angularVelocity->z/1.1;
    }
    
    updatePosition();
}
