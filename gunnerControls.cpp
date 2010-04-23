
#include "gunnerControls.h"

GunnerControls::GunnerControls(InputState *inputState, Camera *cam 
                                            , bool fireWithMouse)
    : inputState(inputState)
    , cam(cam)
    , enabled(true)
    , isFire(false)
    , isShowControls(false)
    , fireKey(OIS::KC_RCONTROL)
    , fireMouseButton(OIS::MB_Left)
    , fireWithMouse(fireWithMouse)
    , minYaw(0)
    , maxYaw(2 * PI)
    , minPitch(0)
    , maxPitch(2 * PI)
    , yaw(0)
    , pitch(0)
{}

void GunnerControls::setYawLimits(float min, float max)
{
    minYaw = min;
    maxYaw = max;
    yaw = minYaw;
}

void GunnerControls::setPitchLimits(float min, float max)
{
    pitch = minPitch;
    minPitch = min;
    maxPitch = max;
}

void GunnerControls::setLook(float yaw, float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
}

bool GunnerControls::fire() {
    return isFire;
}

bool GunnerControls::showControls() {
    return isShowControls;
}

Vector3 GunnerControls::cameraPosition() {
    return cam->getRealPosition();
}

Quaternion GunnerControls::cameraOrientation() {
    return cam->getRealOrientation();
}

void GunnerControls::setEnabled(bool b)
{
    enabled = b;
}

bool GunnerControls::isEnabled() { return enabled; }

void GunnerControls::tickGunnerControls()
{ 
    if(enabled) {
        int x = inputState->getMouseX();
        int y = inputState->getMouseY();
        
        yaw = moveAngleWithLimits(yaw, 
            ConstManager::getFloat("ship_turrent_speed") * x,
            minYaw, maxYaw);
            
        pitch = moveAngleWithLimits(pitch,
            ConstManager::getFloat("ship_turrent_speed") * y,
            minPitch, maxPitch);
        
        // Update camera orientation
        cam->setOrientation(Quaternion());
        cam->yaw(Radian(yaw));
        cam->pitch(Radian(pitch));
        
        if(fireWithMouse) {
            isFire = inputState->isMouseButtonDown(fireMouseButton);
        } else {
            isFire = inputState->isKeyDown(fireKey);
        }
    }

    isShowControls = inputState->isKeyDown(OIS::KC_F1);
}

float GunnerControls::moveAngleWithLimits(float old, float d, 
                                            float min, float max)
{
    old += d;
    
    while(old < 0) old += 2 * PI;
    while(old >= 2* PI) old -= 2 * PI;
    
    bool minFail = min > old;
    bool maxFail = old > max;
    
    bool fail = (min > max) ? minFail && maxFail 
        : minFail || maxFail;
    
    float dMax = abs(old - max);
    float dMin = abs(old - min);
    
    while(dMax > PI) dMax = 2 * PI - dMax;
    while(dMin > PI) dMin = 2 * PI - dMin;
    
    float limit = dMax > dMin ? min : max;

    old = fail ? limit : old;
    
    return old;
}                             
