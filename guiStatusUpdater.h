#ifndef GUI_STATUS_UPDATER_H
#define GUI_STATUS_UPDATER_H

#include "ITickable.h"
#include "guiManager.h"
#include "stateUpdate.h"
#include "damageState.h"
#include "navigatorControls.h"
#include "gameRole.h"
#include "systemManager.h"
#include <sstream>
#include "hud.h"

class GuiStatusUpdater : public ITickable {
    private:
        GuiManager *guiMgr;
        StateUpdate *stateUpdate;
        DamageState *damageState;
        NavigatorControls *navigatorControls;
        GameRole gameRole;
        SystemManager *systemManager;
        HUD *hud;
    public:
        GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState, NavigatorControls *navigatorControls, GameRole gameRole, SystemManager *systemManger, HUD *hud);
        ~GuiStatusUpdater();
        virtual void tick();
};

#endif
