
#include "audioState.h"

AudioState::AudioState(FrontGunState *frontGunState, SoundManager *sndMgr, SceneNode *shipNode)
    : frontGunState(frontGunState),
      sndMgr(sndMgr),
      shipNode(shipNode)
{}

void AudioState::tick()
{
    if(frontGunState->fire()) {
        std::cout << "Bang!" << std::endl;
        sndMgr->playSound(0,shipNode,shipNode);
    }
}

AudioState::~AudioState() {}
