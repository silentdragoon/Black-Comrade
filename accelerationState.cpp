#include "accelerationState.h"

AccelerationState::AccelerationState( IInput *fromInput ):
    input(fromInput),
    vPropForwardVel(0.0),
    vPropSideVel(0.0),
    vPropUpVel(0.0),
    vPropYawVel(0.0),
    vPropPitchVel(0.0)
{}
