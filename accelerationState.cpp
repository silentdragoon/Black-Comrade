#include "accelerationState.h"

AccelerationState::AccelerationState( IAccelerationState *fromInput ):
    input(fromInput),
    vPropForwardVel(0.0),
    vPropSideVel(0.0),
    vPropUpVel(0.0),
    vPropYawVel(0.0),
    vPropPitchVel(0.0),
    cForward(0.0),
    cSide(0.0),
    cUp(0.0),
    cYaw(0.0),
    cPitch(0.0)
{}

double AccelerationState::forward()
{
    return vPropForwardVel;
}

double AccelerationState::side()
{
    return vPropSideVel;
}

double AccelerationState::up()
{
    return vPropUpVel;
}

double AccelerationState::yaw()
{
    return vPropYawVel;
}

double AccelerationState::pitch()
{
    return vPropPitchVel;
}

double calc( double val, double aConst )
{
    if( val > 0 ) return (-exp(-val/(0.35*aConst)) + 1);
    else return -(-exp(val/(0.35*aConst)) + 1);
}

void doTick( double in, double &counter, double &propVel, double BRAKE, double ACCEL)
{
    if( in == 0 ) counter = BRAKE * counter;
    else counter += in;
    propVel = calc( counter, ACCEL);
}



void AccelerationState::tick()
{
    doTick(input->forward(), cForward, vPropForwardVel, 0.97, TICKSFORWARD);

    doTick(input->side(), cSide, vPropSideVel, 0.9, TICKSSIDE);
    
    doTick(input->up(), cUp, vPropUpVel, 0.95, TICKSUP);
    
    doTick(input->yaw(), cYaw, vPropYawVel, 0.9, TICKSYAW);
    
    doTick(input->pitch(), cPitch, vPropPitchVel, 0.95, TICKSPITCH);
    
    /*    
    double tmp = input->forward();
    if( tmp == 0 ) cForward = 0.9 * cForward;
    cForward += tmp;
    vPropForwardVel = calc( cForward, TICKSFORWARD );
    
    tmp = input->side();
    if( tmp == 0 ) cSide = 0.9 * cSide;
    cSide += tmp;
    vPropSideVel = calc( cSide, TICKSSIDE );
    
    tmp = input->up();
    if( tmp == 0 ) cUp = 0.9 * cUp;
    cUp += tmp;
    vPropUpVel = calc( cUp, TICKSUP );
    
    tmp = input->yaw();
    if( tmp == 0 ) cYaw = 0.6 * cYaw;
    cYaw += tmp;
    vPropYawVel = calc( cYaw, TICKSYAW );
    
    tmp = input->pitch();
    if( tmp == 0 ) cPitch = 0.6 * cPitch;
    cPitch += tmp;
    vPropPitchVel = calc( cPitch, TICKSYAW );
    */
}
