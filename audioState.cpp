
#include "audioState.h"

AudioState::AudioState(FrontGunState *frontGunState)
    : frontGunState(frontGunState)
{}

void AudioState::tick()
{
    if(frontGunState->fire())
        std::cout << "Bang!" << std::endl;
}

AudioState::~AudioState() {}
