#include "soundManager.h"

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

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
    shipState = 0;
    engineOn = true;

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
        //errCheck(chan->addDSP(reverb,0),"Adding reverb");
        inactiveChannels.push_back(chan);
    }

    Ogre::LogManager::getSingleton().logMessage("FMODEX OK.");
}

void SoundManager::loadSoundFiles() {
    FMOD::Sound *tempsound;
    // Frontgun sound
    loadSoundFile("sounds/shipgun1.wav",stringToInt("sound_frontgun"),false,false);

    // Frontgun sound
    loadSoundFile("sounds/enemygun1.wav",stringToInt("sound_enemygun"),false,false);

    // Attack mode
    loadSoundFile("sounds/vo/ship/incomingswarms.mp3",stringToInt("sound_incomingswarms"),false,true);

    // Hull critica
    loadSoundFile("sounds/vo/ship/shiphullfilling.mp3",stringToInt("sound_hullfailing"),false,true);

    // Console key press
    loadSoundFile("sounds/consolekeypress.wav",stringToInt("sound_consolekeypress"), false,true);

    // Enemy explosion
    loadSoundFile("sounds/explosion6.wav",stringToInt("sound_explosion"), false,false);

    // Alarm
    loadSoundFile("sounds/alarm_2.wav",stringToInt("sound_alarm"),false,true);

	// English Voiceover Incoming...
	
	loadSoundFile("sounds/vo/engship/10.mp3",stringToInt("sound_Count10"),false,true); //
	loadSoundFile("sounds/vo/engship/15.mp3",stringToInt("sound_Count15"),false,true); //
	loadSoundFile("sounds/vo/engship/30.mp3",stringToInt("sound_Count30"),false,true); //
	loadSoundFile("sounds/vo/engship/54321.mp3",stringToInt("sound_Count54321"),false,true); //
	loadSoundFile("sounds/vo/engship/60.mp3",stringToInt("sound_Count60"),false,true); //
	loadSoundFile("sounds/vo/engship/ama.mp3",stringToInt("sound_AssaultModeActivated"),false,true);
	loadSoundFile("sounds/vo/engship/amd.mp3",stringToInt("sound_AssaultModeDeactivated"),false,true);
	loadSoundFile("sounds/vo/engship/asian.mp3",stringToInt("sound_AsianModeActivated"),false,true);
	loadSoundFile("sounds/vo/engship/bc1.mp3",stringToInt("sound_CommandOrdersBCDestruction"),false,true);
	loadSoundFile("sounds/vo/engship/bc2.mp3",stringToInt("sound_CommandOrdersBCDestruction2"),false,true);
	loadSoundFile("sounds/vo/engship/bcd.mp3",stringToInt("sound_ObjectiveDestroyed2"),false,true); //
	loadSoundFile("sounds/vo/engship/cleared.mp3",stringToInt("sound_ClearedToProceed"),false,true); //
	loadSoundFile("sounds/vo/engship/complete.mp3",stringToInt("sound_MissionComplete"),false,true); //
	loadSoundFile("sounds/vo/engship/destination.mp3",stringToInt("sound_DestinationReached"),false,true);
	loadSoundFile("sounds/vo/engship/detected.mp3",stringToInt("sound_IncomingSwarms2"),false,true); //
	loadSoundFile("sounds/vo/engship/disobey1.mp3",stringToInt("sound_Disobey1"),false,true); //
	loadSoundFile("sounds/vo/engship/disobey2.mp3",stringToInt("sound_Disobey2"),false,true); //
	loadSoundFile("sounds/vo/engship/ef.mp3",stringToInt("sound_EngineDamage2"),false,true); //
	loadSoundFile("sounds/vo/engship/english.mp3",stringToInt("sound_EnglishModeActivated"),false,true);
	loadSoundFile("sounds/vo/engship/enr.mp3",stringToInt("sound_EngineDamage1"),false,true); //
	loadSoundFile("sounds/vo/engship/etsd.mp3",stringToInt("sound_EngineDamage3"),false,true); //
	loadSoundFile("sounds/vo/engship/gameover.mp3",stringToInt("sound_GameOver"),false,true); //
	loadSoundFile("sounds/vo/engship/hdc.mp3",stringToInt("sound_HullDamage1"),false,true); //
	loadSoundFile("sounds/vo/engship/hdi.mp3",stringToInt("sound_HullDamage2"),false,true); //
	loadSoundFile("sounds/vo/engship/hif.mp3",stringToInt("sound_HullDamage3"),false,true); //
	loadSoundFile("sounds/vo/engship/htsd.mp3",stringToInt("sound_HullDamage4"),false,true); //
	loadSoundFile("sounds/vo/engship/nearbc.mp3",stringToInt("sound_NearingBC"),false,true); 
	loadSoundFile("sounds/vo/engship/nominal.mp3",stringToInt("sound_Startup"),false,true); //
	loadSoundFile("sounds/vo/engship/nor.mp3",stringToInt("sound_NewObjectivesReceived"),false,true);
	loadSoundFile("sounds/vo/engship/od.mp3",stringToInt("sound_ObjectiveDestroyed1"),false,true); //
	loadSoundFile("sounds/vo/engship/rd.mp3",stringToInt("sound_ReactorDestroyed"),false,true);
	loadSoundFile("sounds/vo/engship/sd.mp3",stringToInt("sound_ShieldsDown"),false,true); //
	loadSoundFile("sounds/vo/engship/sgnr.mp3",stringToInt("sound_ShieldGenDamage1"),false,true);
	loadSoundFile("sounds/vo/engship/sgtsd.mp3",stringToInt("sound_ShieldGenDamage2"),false,true);
	loadSoundFile("sounds/vo/engship/shf.mp3",stringToInt("sound_ShieldGenFailing"),false,true);
	loadSoundFile("sounds/vo/engship/sma.mp3",stringToInt("sound_StealthModeActivated"),false,true);
	loadSoundFile("sounds/vo/engship/smd.mp3",stringToInt("sound_StealthModeDeactivated"),false,true);
	loadSoundFile("sounds/vo/engship/ssf.mp3",stringToInt("sound_SensorDamage1"),false,true); //
	loadSoundFile("sounds/vo/engship/sstsd.mp3",stringToInt("sound_SensorDamage2"),false,true); //
	loadSoundFile("sounds/vo/engship/swarms.mp3",stringToInt("sound_IncomingSwarms1"),false,true); //
	loadSoundFile("sounds/vo/engship/wcd.mp3",stringToInt("sound_WeaponChargeDepleted"),false,true); //
	loadSoundFile("sounds/vo/engship/wnr.mp3",stringToInt("sound_WeaponDamage5"),false,true); //
	loadSoundFile("sounds/vo/engship/wsdc.mp3",stringToInt("sound_WeaponDamage1"),false,true); //
	loadSoundFile("sounds/vo/engship/wsdi.mp3",stringToInt("sound_WeaponDamage2"),false,true); //
	loadSoundFile("sounds/vo/engship/wsif.mp3",stringToInt("sound_WeaponDamage3"),false,true); //
	loadSoundFile("sounds/vo/engship/wstsd.mp3",stringToInt("sound_WeaponDamage4"),false,true); //

	// Tutorial Voiceover Incoming...

    loadSoundFile("sounds/vo/t/common-01.mp3",stringToInt("sound_common-01"),false,true);
	loadSoundFile("sounds/vo/t/common-02.mp3",stringToInt("sound_common-02"),false,true);
	loadSoundFile("sounds/vo/t/common-03.mp3",stringToInt("sound_common-03"),false,true);
	loadSoundFile("sounds/vo/t/common-04.mp3",stringToInt("sound_common-04"),false,true);
	loadSoundFile("sounds/vo/t/common-05.mp3",stringToInt("sound_common-05"),false,true);
	loadSoundFile("sounds/vo/t/common-06.mp3",stringToInt("sound_common-06"),false,true);
	loadSoundFile("sounds/vo/t/common-07.mp3",stringToInt("sound_common-07"),false,true);
	loadSoundFile("sounds/vo/t/common-08.mp3",stringToInt("sound_common-08"),false,true);
	loadSoundFile("sounds/vo/t/common-09.mp3",stringToInt("sound_common-09"),false,true);
	loadSoundFile("sounds/vo/t/common-10.mp3",stringToInt("sound_common-10"),false,true);
	loadSoundFile("sounds/vo/t/engineer-01.mp3",stringToInt("sound_engineer-01"),false,true);
	loadSoundFile("sounds/vo/t/engineer-02.mp3",stringToInt("sound_engineer-02"),false,true);
	loadSoundFile("sounds/vo/t/engineer-03.mp3",stringToInt("sound_engineer-03"),false,true);
	loadSoundFile("sounds/vo/t/engineer-04.mp3",stringToInt("sound_engineer-04"),false,true);
	loadSoundFile("sounds/vo/t/engineer-05.mp3",stringToInt("sound_engineer-05"),false,true);
	loadSoundFile("sounds/vo/t/engineer-06.mp3",stringToInt("sound_engineer-06"),false,true);
	loadSoundFile("sounds/vo/t/engineer-07.mp3",stringToInt("sound_engineer-07lol"),false,true);
	loadSoundFile("sounds/vo/t/navigator-01.mp3",stringToInt("sound_navigator-01"),false,true);
	loadSoundFile("sounds/vo/t/navigator-02.mp3",stringToInt("sound_navigator-02"),false,true);
	loadSoundFile("sounds/vo/t/navigator-03.mp3",stringToInt("sound_navigator-03"),false,true);
	loadSoundFile("sounds/vo/t/navigator-04.mp3",stringToInt("sound_navigator-04"),false,true);
	loadSoundFile("sounds/vo/t/navigator-05.mp3",stringToInt("sound_navigator-05"),false,true);
	loadSoundFile("sounds/vo/t/navigator-06.mp3",stringToInt("sound_navigator-06"),false,true);
	loadSoundFile("sounds/vo/t/pilot-01.mp3",stringToInt("sound_pilot-01"),false,true);
	loadSoundFile("sounds/vo/t/pilot-02.mp3",stringToInt("sound_pilot-02"),false,true);
	loadSoundFile("sounds/vo/t/pilot-03.mp3",stringToInt("sound_pilot-03"),false,true);
	loadSoundFile("sounds/vo/t/pilot-04.mp3",stringToInt("sound_pilot-04"),false,true);

	// loading Jasmine Aural Patches (or JAP)

	loadSoundFile("sounds/vo/fix/closelong.mp3",stringToInt("sound_closelong"),false,true);
    loadSoundFile("sounds/vo/fix/closeshort.mp3",stringToInt("sound_closeshort"),false,true);
    loadSoundFile("sounds/vo/fix/fixengines.mp3",stringToInt("sound_fixengines"),false,true);
    loadSoundFile("sounds/vo/fix/fixsensors.mp3",stringToInt("sound_fixsensors"),false,true);
    loadSoundFile("sounds/vo/fix/fixweapons.mp3",stringToInt("sound_fixweapons"),false,true);
    loadSoundFile("sounds/vo/fix/readyrun.mp3",stringToInt("sound_readyrun"),false,true);
    loadSoundFile("sounds/vo/fix/tryflying.mp3",stringToInt("sound_tryflying"),false,true);
    loadSoundFile("sounds/vo/fix/tryopeningconsole.mp3",stringToInt("sound_tryopeningconsole"),false,true);
    loadSoundFile("sounds/vo/fix/tryplotting.mp3",stringToInt("sound_tryplotting"),false,true);
    loadSoundFile("sounds/vo/fix/tryradar.mp3",stringToInt("sound_tryradar"),false,true);
    loadSoundFile("sounds/vo/fix/tryrecharge.mp3",stringToInt("sound_tryrecharge"),false,true);

    // loading JAP2

    loadSoundFile("sounds/vo/fix/blastdoors.mp3",stringToInt("sound_blastdoors"),false,true); //
    loadSoundFile("sounds/vo/fix/closelonger.mp3",stringToInt("sound_closelonger"),false,true); //
    loadSoundFile("sounds/vo/fix/tryshowlargeradar.mp3",stringToInt("sound_tryshowlargeradar"),false,true); //
    loadSoundFile("sounds/vo/fix/tryshowlargemap.mp3",stringToInt("sound_tryshowlargemap"),false,true); //
    loadSoundFile("sounds/vo/fix/closemap.mp3",stringToInt("sound_closemap"),false,true); //
    loadSoundFile("sounds/vo/fix/closeradar.mp3",stringToInt("sound_closeradar"),false,true); //
    loadSoundFile("sounds/vo/fix/wait.mp3",stringToInt("sound_wait"),false,true); //
    loadSoundFile("sounds/vo/fix/cheat.mp3",stringToInt("sound_cheat"),false,true);

    // Music section
    loadMusic();

    // Permanent sounds
    loadPermanent();
}

void SoundManager::loadSoundFile(string relativePath, int constName, bool loop, bool twod) {
    FMOD::Sound *tempsound;
    string soundsPath = ConstManager::getString("sound_file_path");

    string fullPath = soundsPath + relativePath;
    if(twod) {
        errCheck(system->createSound(fullPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &tempsound),fullPath);
    } else {
        errCheck(system->createSound(fullPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &tempsound),fullPath);
    }
    if (!loop) {
        errCheck(tempsound->setMode(FMOD_LOOP_OFF),fullPath);
    } else {
        errCheck(tempsound->setMode(FMOD_LOOP_NORMAL),fullPath);
    }
    sounds.insert(pair<int,FMOD::Sound*>(constName,tempsound));
}

void SoundManager::loadMusic() {
    string musicPath = ConstManager::getString("sound_file_path") + "sounds/stealth.wav";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &stealthMusic));
    errCheck(stealthMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "sounds/attack.wav";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &attackMusic));
    errCheck(attackMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "sounds/flee.wav";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &fleeMusic));
    errCheck(fleeMusic->setMode(FMOD_LOOP_OFF));

    musicPath = ConstManager::getString("sound_file_path") + "sounds/menu.wav";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &menuMusic));
    errCheck(menuMusic->setMode(FMOD_LOOP_NORMAL));

    musicPath = ConstManager::getString("sound_file_path") + "sounds/credits.wav";
    errCheck(system->createStream(musicPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &creditMusic));
    errCheck(creditMusic->setMode(FMOD_LOOP_NORMAL));

    // Start the musics
    errCheck(system->playSound(FMOD_CHANNEL_FREE,stealthMusic,true,&stealthChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,attackMusic,true,&attackChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,fleeMusic,true,&fleeChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,menuMusic,true,&menuChannel));
    errCheck(system->playSound(FMOD_CHANNEL_FREE,creditMusic,true,&creditChannel));
    errCheck(stealthChannel->setVolume(0.0));
    errCheck(attackChannel->setVolume(0.0));
    errCheck(fleeChannel->setVolume(0.0));
    errCheck(menuChannel->setVolume(ConstManager::getFloat("sound_musicvolume")));
    errCheck(creditChannel->setVolume(0.0));

    errCheck(stealthChannel->setPaused(false));
    errCheck(attackChannel->setPaused(false));
    errCheck(fleeChannel->setPaused(true));
    errCheck(menuChannel->setPaused(false));
    errCheck(creditChannel->setPaused(false));
}

void SoundManager::loadPermanent() {
    // Engine sound
    string soundsPath = ConstManager::getString("sound_file_path");
    string fullPath = soundsPath + "sounds/engine.wav";
    std::cout << fullPath << std::endl;
    errCheck(system->createSound(fullPath.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D), 0, &engineSound),"create engine sound");
    errCheck(engineSound->setMode(FMOD_LOOP_NORMAL), "engine sound loop");
    errCheck(system->playSound(FMOD_CHANNEL_FREE,engineSound,true,&engineChannel));
    errCheck(engineChannel->setVolume(0.05));
    errCheck(engineChannel->getFrequency(&engineFrequency),"engine_freq_1");
}

void SoundManager::playSound(string constNameString, SceneNode *soundNode, float volume) {
    playSound(constNameString,soundNode->getPosition(),volume);
}

void SoundManager::playSound(string constNameString, Vector3 soundPos, float volume) {
    int constName = stringToInt(constNameString);

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

        channel1->set3DAttributes(&pos, &vel);
        //errCheck( channel1->set3DAttributes(&pos, &vel) );

        errCheck( channel1->setVolume(volume) , constNameString);

        errCheck( channel1->setPaused(false) );
        activeChannels.push_back(channel1);
    }
}

void SoundManager::changeMusic(MusicState file) {
    playingSound = file;
}

void SoundManager::crossFade() {
    double maxVol = ConstManager::getFloat("sound_musicvolume");

    float stealthAdjust;
    float attackAdjust;
    float fleeAdjust;
    float menuAdjust;
    float creditAdjust;
    if(playingSound==MS_STEALTH) {
        stealthAdjust = 0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        menuAdjust = -0.001;
        creditAdjust = -0.001;
    } else if(playingSound==MS_ATTACK) {
        stealthAdjust = -0.001;
        attackAdjust = 0.001;
        fleeAdjust = -0.001;
        menuAdjust = -0.001;
        creditAdjust = -0.001;
    } else if(playingSound==MS_FLEE) {
        errCheck(fleeChannel->setPaused(false));
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = 0.001;
        menuAdjust = -0.001;
        creditAdjust = -0.001;
    } else if(playingSound==MS_MENU) {
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        menuAdjust = 0.001;
        creditAdjust = -0.001;
    } else if(playingSound==MS_CREDITS) {
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        menuAdjust = -0.001;
        creditAdjust = 0.001;
    } else {
        stealthAdjust = -0.001;
        attackAdjust = -0.001;
        fleeAdjust = -0.001;
        menuAdjust = -0.001;
        creditAdjust = 0.001;
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
    if(volume>maxVol+0.3) volume = maxVol+0.3; // LOUDER FOR ESCAPE
    if(volume<0.0) volume = 0.0;
    errCheck(fleeChannel->setVolume(volume));

    errCheck(menuChannel->getVolume(&volume));
    volume=volume+menuAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(menuChannel->setVolume(volume));

    errCheck(creditChannel->getVolume(&volume));
    volume=volume+creditAdjust;
    if(volume>maxVol) volume = maxVol;
    if(volume<0.0) volume = 0.0;
    errCheck(creditChannel->setVolume(volume));
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

void SoundManager::updateEnginePitch() {
    if((shipState!=0)&&(engineOn)) {
        errCheck(engineChannel->setPaused(false));
        double speed = shipState->getSpeed() / 3.6 / 200;
        speed += 1.0;
        float freq = engineFrequency*speed;
        errCheck( engineChannel->setFrequency(freq), "engine pitch"); 
    }
}

void SoundManager::stopEngine() {
    engineOn = false;
    engineChannel->stop();
}

void SoundManager::setShipNode(SceneNode *ship) { shipNode = ship; }
void SoundManager::setShipState(ShipState *state) { shipState = state; }

int SoundManager::stringToInt(string name) {
    int total = 0;
    for(int i=0;i<name.length();i++) {
        total += (int)name[i];
    }
    return total;
}


void SoundManager::tick() {
    crossFade();
    checkChannels();
    updateEnginePitch();
    updateShipPosition();
    errCheck( system->update(),"System Update");
}

SoundManager::~SoundManager() {
    Ogre::LogManager::getSingleton().logMessage("Closing FMODEX sound system...");
    FMOD::Channel *current;
    while(!activeChannels.empty()) {
        bool playing = true;
        current = activeChannels.front();
        current->stop();
        current->isPlaying(&playing);
        activeChannels.pop_front();
        if(playing) {
            activeChannels.push_back(current);
        } else {
            inactiveChannels.push_back(current);
        }
    }
    while(!inactiveChannels.empty()) {
        current = inactiveChannels.front();
        inactiveChannels.pop_front();
        current->stop();
    }
    // Release sounds
    std::map<int,FMOD::Sound*>::iterator it;
    for(it=sounds.begin();it!=sounds.end();it++) {
        FMOD::Sound *s = (*it).second;
        s->release();
    }
    // Stop music
    stealthChannel->stop();
    attackChannel->stop();
    fleeChannel->stop();
    menuChannel->stop();
    creditChannel->stop();
    stealthMusic->release();
    attackMusic->release();
    fleeMusic->release();
    menuMusic->release();
    creditMusic->release();
    // Stop engine noise
    engineChannel->stop();
    engineSound->release();
    //Release system
    errCheck(system->release());
    SLEEP(5);
    Ogre::LogManager::getSingleton().logMessage("FMODEX closed.");
}
