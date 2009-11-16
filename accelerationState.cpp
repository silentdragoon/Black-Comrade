#include "accelerationState.h"

AccelerationState::AccelerationState( IAccelerationState *fromInput ):
    input(fromInput),
    vPropForwardVel(0.0),
    vPropSideVel(0.0),
    vPropUpVel(0.0),
    vPropYawVel(0.0),
    vPropPitchVel(0.0)
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

void AccelerationState::tick()
{
    
    vPropForwardVel = input->forward();
    vPropSideVel = input->side();
    vPropUpVel = input->up();
    vPropYawVel = input->yaw();
    vPropPitchVel = input->pitch();
    
    
}
