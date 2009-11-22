
#ifndef AUDIO_STATE_H
#define AUDIO_STATE_H

#define OIS_DYNAMIC_LIB
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "frontGunState.h"

class AudioState : public ITickable
{
    private:
        FrontGunState *frontGunState;
        
    public:
        
        void tick();
        
        AudioState(FrontGunState *frontGunState);
        ~AudioState();
};

#endif
