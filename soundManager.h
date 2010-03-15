#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <fmod_errors.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include <map>
#include "ITickable.h"
#include "const.h"
#include "constManager.h"

#define MAX_SOUND_CHANNELS  200

using namespace Ogre;
using namespace std;

class SoundManager : public ITickable
{
private:

    std::map<int,FMOD::Sound*> sounds;

    FMOD::System *system;

    // Pointers to the music sounds
    FMOD::Sound *stealthMusic;
    FMOD::Sound *attackMusic;
    FMOD::Sound *fleeMusic;
    FMOD::Sound *themeMusic;
    FMOD::Channel *stealthChannel;
    FMOD::Channel *attackChannel;
    FMOD::Channel *fleeChannel;
    FMOD::Channel *themeChannel;

    // Things for stuff
    int playingSound;

    void errCheck(FMOD_RESULT result);
    void loadSoundFiles();
    void loadMusic();
    void loadSoundFile(string relativePath, int constName, bool loop);
    void crossFade();

public:
    SoundManager();
    ~SoundManager();

    virtual void tick();

    void playSound(int file, SceneNode *shipNode, SceneNode *soundNode, float volume, bool reverb);
    void changeMusic(int file);
};

#endif 
