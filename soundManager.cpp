#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        cerr << "FMOD Error! " << result << " " << FMOD_ErrorString(result) << endl;
    }
}

SoundManager::SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Starting sound system...");
    FMOD_RESULT result;

    result = FMOD::System_Create(&system);
    errCheck(result);

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errCheck(result);

    result = system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
    errCheck(result);

    loadSoundFiles();
    
    Ogre::LogManager::getSingleton().logMessage("Sound system OK.");
}

void SoundManager::loadSoundFiles() {
    FMOD_RESULT result;

    result = system->createSound("./sounds/shipgun1.wav", FMOD_SOFTWARE | FMOD_3D, 0, &sound1);
    errCheck(result);

    result = sound1->set3DMinMaxDistance(4.0f, 10000.0f);
    errCheck(result);

    result = sound1->setMode(FMOD_LOOP_OFF);
    errCheck(result);
}

void SoundManager::playSound(int file, SceneNode *node) {
    FMOD_RESULT result;
    Vector3 place = node->getPosition();

    FMOD_VECTOR pos = {place.x, place.y, place.z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};

    result = system->playSound(FMOD_CHANNEL_FREE, sound1, true, &channel1);
    errCheck(result);
    
    result = channel1->set3DAttributes(&pos, &vel);
    errCheck(result);

    result = channel1->setPaused(false);
    errCheck(result);
}

SoundManager::~SoundManager() {
    errCheck(sound1->release());
    errCheck(system->close());
    errCheck(system->release());

    delete system;
    delete sound1;
    delete channel1;
}
    