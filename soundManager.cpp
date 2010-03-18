#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        cerr << "FMOD Error! " << result << " " << FMOD_ErrorString(result) << endl;
    }
}

SoundManager::SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Starting FMODEX sound system...");

    errCheck( FMOD::System_Create(&system) );

    errCheck( system->setDriver(0) );

    errCheck( system->setOutput(FMOD_OUTPUTTYPE_ALSA) );

    errCheck( system->setSoftwareChannels(100) );

    errCheck( system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0) );

    loadSoundFiles();

    playingSound = 4; // We are playing the theme music at the start
    
    Ogre::LogManager::getSingleton().logMessage("FMODEX OK.");
}

void SoundManager::loadSoundFiles() {
    FMOD::Sound *tempsound;
    // Frontgun sound
    loadSoundFile("/sounds/shipgun1.wav",ConstManager::getInt("sound_frontgun"),false);

    // Frontgun sound
    loadSoundFile("/sounds/enemygun1.wav",ConstManager::getInt("sound_enemygun"),false);

    // Attack mode
    loadSoundFile("/sounds/vo/ship/incomingswarms.mp3",ConstManager::getInt("sound_incomingswarms"),false);

    // Hull critical
    loadSoundFile("/sounds/vo/ship/shiphullfilling.mp3",ConstManager::getInt("sound_hullfailing"),false);

    // Console key press
    loadSoundFile("/sounds/consolekeypress.wav",ConstManager::getInt("sound_consolekeypress"), false);

    // Music section
    loadMusic();
}

void SoundManager::loadSoundFile(string relativePath, int constName, bool loop) {
    FMOD::Sound *tempsound;
    string soundsPath = ConstManager::getString("sound_file_path");

    string fullPath = soundsPath + relativePath;
    errCheck(system->createSound(fullPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &tempsound));
    if (!loop) {
        errCheck(tempsound->setMode(FMOD_LOOP_OFF));
    } else {
        errCheck(tempsound->setMode(FMOD_LOOP_NORMAL));
    }
    sounds.insert(pair<int,FMOD::Sound*>(constName,tempsound));
}

void SoundManager::loadMusic() {
    string musicPath = ConstManager::getString("sound_file_path") + "/sounds/stealth.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &stealthMusic));
    errCheck(stealthMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/attack.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &attackMusic));
    errCheck(attackMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/flee.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &fleeMusic));
    errCheck(fleeMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/theme.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &themeMusic));
    errCheck(themeMusic->setMode(FMOD_LOOP_NORMAL));

    // Start the musics
    errCheck(system->playSound(FMOD_CHANNEL_FREE,stealthMusic,true,&stealthChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,attackMusic,true,&attackChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,fleeMusic,true,&fleeChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,themeMusic,true,&themeChannel));
    errCheck(stealthChannel->setVolume(0.0));
    errCheck(attackChannel->setVolume(0.0));
    errCheck(fleeChannel->setVolume(0.0));
    errCheck(themeChannel->setVolume(0.5));

    errCheck(stealthChannel->setPaused(false));
    errCheck(attackChannel->setPaused(false));
    errCheck(fleeChannel->setPaused(false));
    errCheck(themeChannel->setPaused(false));
}

void SoundManager::playSound(int constName, SceneNode *shipNode, SceneNode *soundNode, float volume, bool reverb) {
    Vector3 shipPos = shipNode->getPosition();
    Vector3 soundPos = soundNode->getPosition();

    //TODO: Take account of rotation
    
    float x = (soundPos.x - shipPos.x) / 50;
    float y = (soundPos.y - shipPos.y) / 50;
    float z = (soundPos.z - shipPos.z) / 50;

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

void SoundManager::changeMusic(int file) {
    // 1: Stealth
    // 2: Attack
    // 3: Flee
    // 4: Theme
    std::cout << "SoumndMGR, MUSIC: " << file << std::endl;
    playingSound = file;
}

void SoundManager::crossFade() {
    double maxVol = ConstManager::getFloat("sound_musicvolume");

    float stealthAdjust;
    float attackAdjust;
    float fleeAdjust;
    float themeAdjust;
    if(playingSound==1) {
        stealthAdjust = 0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        themeAdjust = -0.001;
    } else if(playingSound==2) {
        stealthAdjust = -0.001;
        attackAdjust = 0.001;
        fleeAdjust = -0.001;
        themeAdjust = -0.001;
    } else if(playingSound==3) {
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = 0.001;
        themeAdjust = -0.001;
    } else {
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        themeAdjust = 0.001;
    }
    float volume;
    errCheck(stealthChannel->getVolume(&volume));
    volume=volume+stealthAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(stealthChannel->setVolume(volume));

    errCheck(attackChannel->getVolume(&volume));
    volume=volume+attackAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(attackChannel->setVolume(volume));

    errCheck(fleeChannel->getVolume(&volume));
    volume=volume+fleeAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(fleeChannel->setVolume(volume));

    errCheck(themeChannel->getVolume(&volume));
    volume=volume+themeAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(themeChannel->setVolume(volume));
}

void SoundManager::tick() {
    crossFade();
    errCheck( system->update() );
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing FMODEX sound system...");
    errCheck(system->release());
    Ogre::LogManager::getSingleton().logMessage("FMODEX closed.");
}
