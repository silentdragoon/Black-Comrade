
#ifndef GUNNER_CONTROLS_H
#define GUNNER_CONTROLS_H

#include <OGRE/Ogre.h>

#include "constManager.h"

#include "inputState.h"
#include "const.h"

using namespace Ogre;

class GunnerControls {

private:

    bool enabled;
    bool isFire;

    Camera *cam;
    InputState *inputState;
    
    OIS::KeyCode fireKey;
    OIS::MouseButtonID fireMouseButton;
    bool fireWithMouse;
    
    float yaw, pitch;
    
    float maxYaw;
    float minYaw;
    
    float maxPitch;
    float minPitch;
    
    float moveAngleWithLimits(float old, float d, 
                              float min, float max);

public:
	
	GunnerControls(InputState *inputState, Camera *cam, 
	    bool fireWithMouse = true);
	
	~GunnerControls() {}
	
	bool fire();
	
    void setEnabled(bool isEnabled);
    bool isEnabled();
    
    void tickGunnerControls();
    
    void setYawLimits(float min, float max);
    void setPitchLimits(float min, float max);
    
    void setLook(float yaw, float pitch);
    
    Vector3 cameraPosition();
    Quaternion cameraOrientation();
};

#endif
