
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

#include "replicaObject.h"

enum Notification { NT_NONE, NT_CONTROLS, NT_ATTACK, NT_HULL_CRITICAL, NT_ENGINES_CRITICAL, NT_WEAPONS_CRITICAL };

class NotificationManager : public ITickable, public ReplicaObject
{
private:
	Notification notification;
    Notification oldNotification;

    std::vector<Notification> queue;
    std::map <Notification,int> recency;

    GameStateMachine *stateMachine;
	MapManager *mapManager;
	ShipState *shipState;
    DamageState *damageState;
	bool mIsNewNotification;

    int tickcount;

    void checkGameState();
    void checkShipPosition();
    void checkHealth();

    int getTimeSince(Notification notification);
    void updateRecencies();
    void notify();
	
public:
    NotificationManager();
	NotificationManager(GameStateMachine *stateMachine, MapManager *mapManager, ShipState *shipState, DamageState *damageState);
	void tick();
	
	Notification currentNotification();
	bool isNewNotification();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
