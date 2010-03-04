
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                       NotificationManager *notificationMgr)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode),
      notificationMgr(notificationMgr)
{}

void AudioState::tick()
{
    if(gunState->fire()) {
        sndMgr->playSound(Const::SOUND_FRONTGUN,shipNode,shipNode,0.5,true);
    }

    if(notificationMgr->hasNewNotification()) {
        Notification *newNotification = notificationMgr->getCurrentNotification();
        if (newNotification->getType() != NT_NONE && newNotification->getSoundNameConst() != -1) {
            sndMgr->playSound(newNotification->getSoundNameConst(),shipNode,shipNode,0.5,true);
        }
    }
}

AudioState::~AudioState() {}
