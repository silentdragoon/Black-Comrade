#ifndef GUI_STATUS_UPDATER_H
#define GUI_STATUS_UPDATER_H

#include "ITickable.h"
#include "guiManager.h"
#include "stateUpdate.h"
#include "damageState.h"
#include "navigatorControls.h"
#include "gunnerControls.h"
#include "gameRole.h"
#include "systemManager.h"
#include <sstream>
#include "hud.h"
#include "flying.h"
#include "notificationManager.h"
#include "gameStateMachine.h"
#include "objective.h"
#include "collaborationInfo.h"

class GuiStatusUpdater : public ITickable {
    private:
        GuiManager *guiMgr;
        StateUpdate *stateUpdate;
        DamageState *damageState;
        GunnerControls *playerControls;
        GameRole gameRole;
        SystemManager *systemManager;
        HUD *hud;
        Flying *flying;
        NotificationManager *notificationMgr;
        GameStateMachine *gameStateMachine;
        GameState lastStateNotified;
        Objective *objective;
        CollaborationInfo *pilotInfo;
        CollaborationInfo *engInfo;
        CollaborationInfo *navInfo;

    public:
        GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState,
                         GunnerControls *playerControls, GameRole gameRole,
                         SystemManager *systemManager, HUD *hud, Flying *flying,
                         NotificationManager *notificationMgr,
                         GameStateMachine *gameStateMachine, Objective *objective,
                         CollaborationInfo *pilotInfo, CollaborationInfo *navInfo, CollaborationInfo *engInfo);

        ~GuiStatusUpdater();
        virtual void tick();
};

#endif
