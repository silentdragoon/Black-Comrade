#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        cerr << "FMOD Error! " << result << " " << FMOD_ErrorString(result) << endl;
    }
}

SoundManager::SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Starting FMODEX sound system...");

    errCheck( FMOD::System_Create(&system) );

    errCheck( system->setOutput(FMOD_OUTPUTTYPE_ALSA) );

    errCheck( system->setSoftwareChannels(100) );

    errCheck( system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0) );

    loadSoundFiles();
    
    Ogre::LogManager::getSingleton().logMessage("FMODEX OK.");
}

void SoundManager::loadSoundFiles() {
    FMOD::Sound *tempsound;

    // Frontgun sound
    errCheck(system->createSound("./sounds/shipgun1.wav", (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &tempsound));
    errCheck(tempsound->setMode(FMOD_LOOP_OFF));
    int vag = Const::SOUND_FRONTGUN;
    sounds.insert(pair<int,FMOD::Sound*>(vag,tempsound));

    // Background music
    errCheck(system->createStream("./sounds/background.mp3", (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &tempsound));
    errCheck(tempsound->setMode(FMOD_LOOP_NORMAL));
    vag = Const::SOUND_BACKGROUNDMUSIC;
    sounds.insert(pair<int,FMOD::Sound*>(vag,tempsound));
}

void SoundManager::playSound(int constName, SceneNode *shipNode, SceneNode *soundNode, float volume, bool reverb) {
    Vector3 shipPos = shipNode->getPosition();
    Vector3 soundPos = soundNode->getPosition();
    
    float x = soundPos.x - shipPos.x;
    float y = soundPos.y - shipPos.y;
    float z = soundPos.z - shipPos.z;

    FMOD_VECTOR pos = {x,y,z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};
    
    FMOD::Channel *channel1;

    FMOD::Sound *temp = sounds[constName];
    errCheck( system->playSound(FMOD_CHANNEL_FREE, temp, true, &channel1) );
    
    errCheck( channel1->set3DAttributes(&pos, &vel) );

    errCheck( channel1->setVolume(volume) );

    if(reverb==true) {
        // I think this works. 
        FMOD::DSP *reverb;
        system->createDSPByType(FMOD_DSP_TYPE_SFXREVERB,&reverb);
        reverb->setParameter(FMOD_DSP_SFXREVERB_DECAYTIME,5.0);
        channel1->addDSP(reverb,0);
    }

    errCheck( channel1->setPaused(false) );
    
}

void SoundManager::tick() {
    errCheck( system->update() );
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing FMODEX sound system...");
    errCheck(system->release());
    Ogre::LogManager::getSingleton().logMessage("FMODEX closed.");
}
