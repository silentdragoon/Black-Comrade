
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                       NotificationManager *notificationMgr, BulletManager *bulletMgr,
                       MiniGameManager *miniGameMgr,GameStateMachine *gameStateMachine,
                       Objective *objective, DamageState *damageState)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode),
      notificationMgr(notificationMgr),
      bulletMgr(bulletMgr),
      miniGameMgr(miniGameMgr),
      gameStateMachine(gameStateMachine),
      objective(objective),
      damageState(damageState),
      prevTime(ConstManager::getInt("escape_time")),
      tickTime(0)
{}

void AudioState::tick()
{
    // Process bullets
    Bullet *b;
    std::vector<Bullet*> *bullets = bulletMgr->getActiveBullets();
   	for(std::vector<Bullet*>::const_iterator it=bullets->begin();it!=bullets->end();++it) {
	    b = *it;
	    if (b->madeNoise) continue;
	    
	    if (b->getOwner()->getEntityType() == ENTT_ENEMY) {
	        // Enemy bullet
	        sndMgr->playSound("sound_enemygun",b->getOrigin(),0.04);
	    } else if (b->getOwner()->getEntityType() == ENTT_PLAYER) {
	        // Player bullet
	        sndMgr->playSound("sound_frontgun",b->getOrigin(),0.1);
	    }
	    
	    b->madeNoise = true;
	}

    if(notificationMgr->hasNewNotification()) {
        Notification *newNotification = notificationMgr->getCurrentNotification();
        if (newNotification->getType() != NT_NONE && newNotification->getSoundNameConst() != "") {
            sndMgr->playSound(newNotification->getSoundNameConst(),shipNode,20);
        }
    }
    
    if(gameStateMachine->isNewState()) {
        GameState state = gameStateMachine->currentGameState();
        switch(state) {
            case GS_STEALTH:
                sndMgr->changeMusic(MS_STEALTH);
                break;
            case GS_ATTACK:
                sndMgr->changeMusic(MS_ATTACK);
                break;
            case GS_FLEE:
                sndMgr->changeMusic(MS_FLEE);
                break;
            case GS_GAME_OVER:
                sndMgr->changeMusic(MS_CREDITS);// TODO: Change to sad music
                break;
            case GS_END:
                sndMgr->changeMusic(MS_CREDITS);// TODO: Change to awesome victory music
                break;
        }
    }

    if (miniGameMgr->aKeyPressed) {
        miniGameMgr->aKeyPressed = false;
        sndMgr->playSound("sound_consolekeypress",shipNode,0.1);
    }

    // Ship alarm for impending doom
    int curTime = objective->getEscapeTime();
    if(curTime!=prevTime) {
        prevTime=curTime;
        if(curTime<=5) {
            sndMgr->playSound("sound_alarm",shipNode,0.2);
        }
    }

    // Ship alarm for damaged hull
    if(damageState->getHullHealth() < 25.0) {
        if(tickTime==0) {
            sndMgr->playSound("sound_alarm",shipNode,0.5);
            tickTime=(int)(1.0/ConstManager::getFloat("tick_period"));
        }
        tickTime--;
    }
    
    if(objective->getHealth() == 0)
    	sndMgr->playSound("sound_bcexplosion",objective->getPosition(),0.5);
}

AudioState::~AudioState() {}
