
#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "ITickable.h"
#include "gameStateMachine.h"
#include "shipState.h"
#include "mapManager.h"
#include "damageState.h"
#include "constManager.h"
#include <vector>
#include <limits>
#include <cstdlib>

#include "replicaObject.h"
#include "collaborationInfo.h"

enum Notification { NT_NONE, NT_CONTROLS, NT_UNDER_ATTACK,
                    NT_HULL_CRITICAL, NT_ENGINES_CRITICAL, NT_WEAPONS_CRITICAL,
                    NT_OBJECTIVE_SEEK, NT_OBJECTIVE_DESTROY, NT_OBJECTIVE_ESCAPE,
                    NT_COMMENT_ONE, NT_COMMENT_TWO, NT_COMMENT_THREE };

class NotificationManager : public ITickable, public ReplicaObject
{
private:
    Notification notification;
    Notification lastNotification;

    std::vector<Notification> queue;
    std::map <Notification,int> recency;

    CollaborationInfo *collabInfo;
    GameStateMachine *stateMachine;
    GameState lastStateNotified;
    MapManager *mapManager;
    ShipState *shipState;
    DamageState *damageState;
    bool mIsNewNotification;

    int tickcount;
    int maxDelay;

    bool controlsDisplayed;
    bool stateChangePending;

    void checkComments();
    void checkGameState();
    void checkShipPosition();
    void checkHealth();

    bool isTimely(Notification notification, int delaySinceMe, int delaySinceLast);
    int getTimeSince(Notification notification);
    int getTimeSinceLast();
    void updateRecencies();
    void notify();
	
public:
    NotificationManager();
    NotificationManager(CollaborationInfo *collabInfo, GameStateMachine *stateMachine,
                        MapManager *mapManager, ShipState *shipState,
                        DamageState *damageState);
    void tick();
	
    Notification getCurrentNotification();
    bool hasNewNotification();

    void setCollaborationInfo(CollaborationInfo *collabInfo);

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
