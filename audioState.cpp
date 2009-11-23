
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
        int gunFireSound;
        int gunFireChannel;
        
        String file = "shipgun1.wav";
        gunFireSound = sndMgr->CreateSound(file);
        sndMgr->PlaySound(gunFireSound, shipNode, &gunFireChannel);
    }
}

AudioState::~AudioState() {}
