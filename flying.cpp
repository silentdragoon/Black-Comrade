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

    v2 = v2*v2; // Square the velocity for a laugh
    cout << "v2: " << v2 << endl;

    double drag = 0.5*Const::AIR_DENSITY*v2*Const::DRAG_COEFFICIENT*Const::REFERENCE_AREA;
    cout << "Drag: " << drag << endl;
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
    position->x=position->x+velocity->x;
    position->y=position->y+velocity->y;
    position->z=position->z+velocity->z;

    cout << position->x << " " << position->y << " " << position->z << endl;

    orientation->x=orientation->x+angularVelocity->x;
    orientation->y=orientation->y+angularVelocity->y;
    orientation->z=orientation->z+angularVelocity->z;
}

void Flying::tick()
{
    if(ks->isKeyDown(OIS::KC_A)) {
        double accel = (Const::SHIP_SIDE_THRUST-getDrag("x"))/Const::SHIP_MASS;
        velocity->x = velocity->x + accel;
    } else if(ks->isKeyDown(OIS::KC_D)) {
        double accel = (-Const::SHIP_SIDE_THRUST-getDrag("x"))/Const::SHIP_MASS;
        velocity->x = velocity->x + accel;
    } else if(ks->isKeyDown(OIS::KC_SPACE)) {
        double accel = (Const::SHIP_UP_THRUST-getDrag("y"))/Const::SHIP_MASS;
        velocity->y = velocity->y + accel;
    } else if(ks->isKeyDown(OIS::KC_LSHIFT)) {
        double accel = (Const::SHIP_DOWN_THRUST-getDrag("y"))/Const::SHIP_MASS;
        velocity->y = velocity->y + accel;
    } else if(ks->isKeyDown(OIS::KC_W)) {
        double accel = (Const::SHIP_FORWARD_THRUST-getDrag("z"))/Const::SHIP_MASS;
        velocity->z = velocity->z + accel;
    } else if(ks->isKeyDown(OIS::KC_S)) {
        double accel = (-Const::SHIP_FORWARD_THRUST-getDrag("z"))/Const::SHIP_MASS;
        velocity->z = velocity->z + accel;
    } else if(ks->isKeyDown(OIS::KC_UP)) {
        double accel = (-Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->x = angularVelocity->x + accel;
    } else if(ks->isKeyDown(OIS::KC_DOWN)) {
        double accel = (Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->x = angularVelocity->x + accel;
    } else if(ks->isKeyDown(OIS::KC_LEFT)) {
        double accel = (Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->z = angularVelocity->z + accel;
    } else if(ks->isKeyDown(OIS::KC_RIGHT)) {
        double accel = (-Const::SHIP_ROTATE_THRUST)/Const::SHIP_MASS;
        angularVelocity->z = angularVelocity->z + accel;
    } else {
        double accelx = -getDrag("x")/Const::SHIP_MASS;
        double accely = -getDrag("y")/Const::SHIP_MASS;
        double accelz = -getDrag("z")/Const::SHIP_MASS;

        velocity->x = velocity->x + accelx;
        velocity->y = velocity->y + accely;
        velocity->z = velocity->z + accelz;
    }
    
    updatePosition();
}
