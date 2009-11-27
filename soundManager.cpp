#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        //cout << "FMOD Error! " << result << " " << FMOD_ErrorString(result) << endl;
        Ogre::LogManager::getSingleton().logMessage("FMOD Error! "+String(result)+" "+FMOD_ErrorString(result));
    }
}

SoundManager::SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Starting sound system...");

    errCheck( FMOD::System_Create(&system) );

    errCheck( system->setOutput(FMOD_OUTPUTTYPE_ALSA) );

    errCheck( system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0) );

    loadSoundFiles();
    
    Ogre::LogManager::getSingleton().logMessage("Sound system OK.");
}

void SoundManager::loadSoundFiles() {
    errCheck( system->createSound("./sounds/shipgun1.wav", FMOD_SOFTWARE | FMOD_3D, 0, &sound1) );

    errCheck( sound1->set3DMinMaxDistance(0.0f, 10000.0f) );

    errCheck( sound1->setMode(FMOD_LOOP_OFF) );
}

void SoundManager::playSound(int file, SceneNode *shipNode, SceneNode *soundNode) {
    Vector3 shipPos = shipNode->getPosition();
    Vector3 soundPos = soundNode->getPosition();
    
    float x = soundPos.x - shipPos.x;
    float y = soundPos.y - shipPos.y;
    float z = soundPos.z - shipPos.z;

    FMOD_VECTOR pos = {x,y,z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};

    errCheck( system->playSound(FMOD_CHANNEL_FREE, sound1, true, &channel1) );
    
    errCheck( channel1->set3DAttributes(&pos, &vel) );

    errCheck( channel1->setPaused(false) );
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing sound system...");
    errCheck(sound1->release());
    errCheck(system->close());
    errCheck(system->release());
    Ogre::LogManager::getSingleton().logMessage("Sound closed.");

    delete system;
    delete sound1;
    delete channel1;
}
    
