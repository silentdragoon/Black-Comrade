#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmodex/fmod.hpp>
//#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
//#include <fmodex/fmodlinux.h>
#include <OGRE/Ogre.h>
#include <iostream>

#define MAX_SOUND_CHANNELS  200

using namespace Ogre;
using namespace std;

class SoundManager
{
private:
    FMOD::System *system;
    FMOD::Sound *sound1;
    FMOD::Channel *channel1;

    void errCheck(FMOD_RESULT result);
    void loadSoundFiles();

public:
    SoundManager();
    ~SoundManager();
    void playSound(int file, SceneNode *shipNode, SceneNode *soundNode);
};

#endif 
