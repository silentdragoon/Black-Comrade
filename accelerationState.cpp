
#include "accelerationState.h"
#include "const.h"

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

void doTick( double in, double &counter, double &propVel, double BRAKE, double FRICK, double ACCEL)
{
    if( in == 0 ) counter = FRICK * counter;
    else
    {
        counter += in;
        if( in > 0 && counter >= 0 ) { if (counter > 2*ACCEL)counter = 2*ACCEL; }
        else if( in < 0 && counter <= 0) { if(counter < -2*ACCEL) counter = -2*ACCEL; }
        else counter = counter * (FRICK*0.95);
    }
    propVel = calc( counter, ACCEL);
}



void AccelerationState::tick()
{
    
    doTick(input->forward(), cForward, vPropForwardVel, 2, 0.97, Const::TICKSFORWARD);

    doTick(input->side(), cSide, vPropSideVel, 4, 0.93, Const::TICKSSIDE);
    
    doTick(input->up(), cUp, vPropUpVel, 4, 0.95, Const::TICKSUP);
    
    doTick(input->yaw(), cYaw, vPropYawVel, 2, 0.9, Const::TICKSYAW);
    
    doTick(input->pitch(), cPitch, vPropPitchVel, 2, 0.95, Const::TICKSPITCH);
    
    /*
    doTick(input->forward(), cForward, vPropForwardVel, 0.97, Const::TICKSFORWARD);

    doTick(input->side(), cSide, vPropSideVel, 0.9, Const::TICKSSIDE);
    
    doTick(input->up(), cUp, vPropUpVel, 0.95, Const::TICKSUP);
    
    doTick(input->yaw(), cYaw, vPropYawVel, 0.9, Const::TICKSYAW);
    
    doTick(input->pitch(), cPitch, vPropPitchVel, 0.95, Const::TICKSPITCH);
    
    
    double tmp = input->forward();
    if( tmp == 0 ) cForward = 0.9 * cForward;
    cForward += tmp;
    vPropForwardVel = calc( cForward, Const::TICKSFORWARD );
    
    tmp = input->side();
    if( tmp == 0 ) cSide = 0.9 * cSide;
    cSide += tmp;
    vPropSideVel = calc( cSide, Const::TICKSSIDE );
    
    tmp = input->up();
    if( tmp == 0 ) cUp = 0.9 * cUp;
    cUp += tmp;
    vPropUpVel = calc( cUp, Const::TICKSUP );
    
    tmp = input->yaw();
    if( tmp == 0 ) cYaw = 0.6 * cYaw;
    cYaw += tmp;
    vPropYawVel = calc( cYaw, Const::TICKSYAW );
    
    tmp = input->pitch();
    if( tmp == 0 ) cPitch = 0.6 * cPitch;
    cPitch += tmp;
    vPropPitchVel = calc( cPitch, Const::TICKSYAW );
    */
}
