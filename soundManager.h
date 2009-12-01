#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <fmod_errors.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"

#define MAX_SOUND_CHANNELS  200

using namespace Ogre;
using namespace std;

class SoundManager : public ITickable
{
private:
    FMOD::System *system;
    FMOD::Sound *sound1;

    void errCheck(FMOD_RESULT result);
    void loadSoundFiles();

public:
    SoundManager();
    ~SoundManager();

    virtual void tick();

    void playSound(int file, SceneNode *shipNode, SceneNode *soundNode, float volume, bool reverb);
};

#endif 
