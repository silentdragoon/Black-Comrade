
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode)
{}

void AudioState::tick()
{
    if(gunState->fire()) {
        sndMgr->playSound(Const::SOUND_FRONTGUN,shipNode,shipNode,0.5,true);
    }
}

AudioState::~AudioState() {}
