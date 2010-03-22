#include "soundManager.h"

void SoundManager::errCheck(FMOD_RESULT result, std::string from) {
    if (result != FMOD_OK) {
        cerr << "FMOD: " << result << " " << FMOD_ErrorString(result) << " : " << from << endl;
    }
}

void SoundManager::errCheck(FMOD_RESULT result) {
    errCheck(result,"");
}

SoundManager::SoundManager() {
    shipNode = 0;

    Ogre::LogManager::getSingleton().logMessage("Starting FMODEX sound system...");

    errCheck( FMOD::Memory_Initialize(malloc(64*1024*1024), 64*1024*1024, 0,0,0) );

    errCheck( FMOD::System_Create(&system) );

    errCheck( system->setDriver(0) );

    errCheck( system->setOutput(FMOD_OUTPUTTYPE_ALSA) );

    errCheck( system->setSoftwareChannels(50) );

    errCheck( system->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0) );

    loadSoundFiles();

    playingSound = 4; // We are playing the theme music at the start

    // Create reverb effect
    errCheck(system->createDSPByType(FMOD_DSP_TYPE_SFXREVERB,&reverb),"Create reverb");
    errCheck(reverb->setParameter(FMOD_DSP_SFXREVERB_DECAYTIME,5.0),"Reverb param");

    for(int i=0;i<50;i++) {
        FMOD::Channel *chan;
        errCheck(chan->addDSP(reverb,0),"Adding reverb");
        inactiveChannels.push_back(chan);
    }

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
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &stealthMusic));
    errCheck(stealthMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/attack.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &attackMusic));
    errCheck(attackMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/flee.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &fleeMusic));
    errCheck(fleeMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "/sounds/theme.mp3";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &themeMusic));
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

void SoundManager::playSound(int constName, SceneNode *soundNode, float volume) {
    playSound(constName,soundNode->getPosition(),volume);
}

void SoundManager::playSound(int constName, Vector3 soundPos, float volume) {

    float x = soundPos.x/50.0;
    float y = soundPos.y/50.0;
    float z = soundPos.z/50.0;

    FMOD_VECTOR pos = {x,y,z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};
    
    //FMOD::Channel *channel1;
    if(!inactiveChannels.empty()) {
        FMOD::Channel *channel1 = inactiveChannels.front();
        inactiveChannels.pop_front();

        errCheck( system->playSound(FMOD_CHANNEL_FREE, sounds[constName], false, &channel1) );

        errCheck( channel1->set3DAttributes(&pos, &vel) );

        errCheck( channel1->setVolume(volume) );

        errCheck( channel1->setPaused(false) );
        activeChannels.push_back(channel1);
    }
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

void SoundManager::checkChannels() {
    FMOD::Channel *current;
    if(!activeChannels.empty()) {
        bool playing = true;
        current = activeChannels.front();
        current->isPlaying(&playing);
        activeChannels.pop_front();
        if(playing) {
            activeChannels.push_back(current);
        } else {
            inactiveChannels.push_back(current);
        }
    }
}

void SoundManager::updateShipPosition() {
    if(shipNode!=0) {
        FMOD_VECTOR position;
        FMOD_VECTOR forward;
        FMOD_VECTOR up;
        FMOD_VECTOR velocity;
        Vector3 vectorForward = shipNode->getOrientation().zAxis();
        Vector3 vectorUp = shipNode->getOrientation().yAxis();
        Vector3 vectorVelocity = Vector3(0,0,0);

        position.x = shipNode->getPosition().x/50.0;
        position.y = shipNode->getPosition().y/50.0;
        position.z = shipNode->getPosition().z/50.0;

        forward.x = vectorForward.x;
        forward.y = vectorForward.y;
        forward.z = vectorForward.z;

        up.x = vectorUp.x;
        up.y = vectorUp.y;
        up.z = vectorUp.z;

        velocity.x = vectorVelocity.x;
        velocity.y = vectorVelocity.y;
        velocity.z = vectorVelocity.z;

        errCheck( system->set3DListenerAttributes(0,&position,&velocity,&forward,&up),"set3DListenerAttributes" );
    }
}

void SoundManager::setShipNode(SceneNode *ship) { shipNode = ship; }

void SoundManager::tick() {
    crossFade();
    checkChannels();
    updateShipPosition();
    errCheck( system->update(),"System Update");
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing FMODEX sound system...");
    errCheck(system->release());
    Ogre::LogManager::getSingleton().logMessage("FMODEX closed.");
}
