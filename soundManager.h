#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <fmod_errors.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include <map>
#include <deque>
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

    std::deque<FMOD::Channel*> activeChannels;
    std::deque<FMOD::Channel*> inactiveChannels;

    FMOD::System *system;
    SceneNode *shipNode;

    // Pointers to the music sounds
    FMOD::Sound *stealthMusic;
    FMOD::Sound *attackMusic;
    FMOD::Sound *fleeMusic;
    FMOD::Sound *themeMusic;
    FMOD::Channel *stealthChannel;
    FMOD::Channel *attackChannel;
    FMOD::Channel *fleeChannel;
    FMOD::Channel *themeChannel;

    // Effects
    FMOD::DSP *reverb;

    // Things for stuff
    int playingSound;

    void errCheck(FMOD_RESULT result);
    void errCheck(FMOD_RESULT result,std::string from);
    void loadSoundFiles();
    void loadMusic();
    void loadSoundFile(string relativePath, int constName, bool loop);
    void crossFade();
    void checkChannels();
    void updateShipPosition();

public:
    SoundManager();
    ~SoundManager();

    virtual void tick();

    void playSound(int file, Vector3 position, float volume);
    void playSound(int file, SceneNode *soundNode, float volume);
    void changeMusic(int file);
    void setShipNode(SceneNode *ship);
};

#endif 
