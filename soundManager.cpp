#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        cerr << "FMOD Error! " << result << " " << FMOD_ErrorString(result) << endl;
    }
}

SoundManager::SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Starting sound system...");

    errCheck( FMOD::System_Create(&system) );

    errCheck( system->setOutput(FMOD_OUTPUTTYPE_ALSA) );

    errCheck( system->setSoftwareChannels(100) );

    errCheck( system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0) );

    FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_SEWERPIPE;
    errCheck( system->setReverbAmbientProperties(&prop) );

    loadSoundFiles();
    
    Ogre::LogManager::getSingleton().logMessage("Sound system OK.");
}

void SoundManager::loadSoundFiles() {
    errCheck( system->createSound("./sounds/shipgun1.wav", (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &sound1) );

    //errCheck( sound1->set3DMinMaxDistance(0.0f, 10000.0f) );

    errCheck( sound1->setMode(FMOD_LOOP_OFF) );
}

void SoundManager::playSound(int file, SceneNode *shipNode, SceneNode *soundNode, float volume) {
    Vector3 shipPos = shipNode->getPosition();
    Vector3 soundPos = soundNode->getPosition();
    
    float x = soundPos.x - shipPos.x;
    float y = soundPos.y - shipPos.y;
    float z = soundPos.z - shipPos.z;

    //cout << x << " " << y << " " << z << endl;

    FMOD_VECTOR pos = {x,y,z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};
    
    FMOD::Channel *channel1;

    errCheck( system->playSound(FMOD_CHANNEL_FREE, sound1, true, &channel1) );
    
    errCheck( channel1->set3DAttributes(&pos, &vel) );

    errCheck( channel1->setVolume(volume) );

    errCheck( channel1->setPaused(false) );
    
}

void SoundManager::tick() {
    errCheck( system->update() );
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing sound system...");
    errCheck(system->release());
    Ogre::LogManager::getSingleton().logMessage("Sound closed.");
}
    
