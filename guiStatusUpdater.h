#ifndef GUI_STATUS_UPDATER_H
#define GUI_STATUS_UPDATER_H

#include "ITickable.h"
#include "guiManager.h"
#include "stateUpdate.h"
#include "damageState.h"
#include <sstream>

class GuiStatusUpdater : public ITickable {
    private:
        GuiManager *guiMgr;
        StateUpdate *stateUpdate;
        DamageState *damageState;
    public:
        GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState);
        ~GuiStatusUpdater();
        virtual void tick();
};

#endif
