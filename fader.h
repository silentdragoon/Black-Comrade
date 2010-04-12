#ifndef FADER
#define FADER

#include "ITickable.h"
#include "guiManager.h"

class Fader : public ITickable {
    private:

        GuiManager *guiMgr;

        bool fadingIn;
        bool fadingOut;

    public:
        Fader(GuiManager *guiMgr);
	virtual void tick();
};

#endif
