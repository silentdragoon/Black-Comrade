#include "fader.h"

Fader::Fader(GuiManager *guiMgr)
    : guiMgr(guiMgr)
    , fadingIn(true)
    , fadingOut(false)
{}

void Fader::tick() {
    if (fadingIn) {
        fadingOut = false;
        fadingIn = !guiMgr->fadeFromBlack(true);
    } else if (fadingOut) {
        fadingIn = false;
        fadingOut = !guiMgr->fadeToBlack();
    }
}
