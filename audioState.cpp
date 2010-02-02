
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode)
{}

void AudioState::tick()
{
    if(gunState->pilotFire()) {
        sndMgr->playSound(0,shipNode,shipNode,0.5,true);
    }
}

AudioState::~AudioState() {}
