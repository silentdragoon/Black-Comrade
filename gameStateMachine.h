
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "ITickable.h"
#include "mapManager2.h"
#include "shipState.h"
#include "replicaObject.h"
#include "damageState.h"
#include "objective.h"
#include "inputState.h"
#include "tutorial.h"

enum GameState { GS_TUTORIAL, GS_STEALTH, GS_ATTACK, GS_BLACK_COMRADE, GS_FLEE, GS_GAME_OVER, GS_END };

class GameStateMachine : public ITickable, public ReplicaObject
{
private:
    bool isShipInSight;
    std::string *waypointName;
    double hullDamage;

    InputState *inputState;
    CollaborationInfo *pilotInfo;
    CollaborationInfo *engInfo;
    CollaborationInfo *navInfo;

    Tutorial *tutorial;

    GameState gameState;
    GameState oldState;

    Objective *objective;
    MapManager *mapManager;
    ShipState *shipState;
    DamageState *damageState;
    bool mIsNewState;

    void checkWaypoints();
    void checkHealth();
    void checkSwarms();
    void checkObjective();
    void checkInput();
    void checkForQuit();
    void checkTutorial();
	
public:
    GameStateMachine();
    GameStateMachine(MapManager *mapManager, InputState *inputState,
                     CollaborationInfo *pilotInfo, CollaborationInfo *engInfo, CollaborationInfo *navInfo,
                     Tutorial *tutorial, ShipState *shipState, DamageState *damageState, Objective *objective);

    void tick();

    void setInputState(InputState *inputState);
    void setInfos(CollaborationInfo *nPilotInfo,
                  CollaborationInfo *nNavInfo,
                  CollaborationInfo *nEngInfo);

    void setIsShipInSight(bool isShipInSight);
    bool getIsShipInSight();

    void setWaypointName(std::string *waypointName);
    std::string* getWaypointName();

    void setHullDamage(double hullDamage);
    double getHullDamage();
	
    GameState currentGameState();
    bool isNewState();

    void setTutorial(Tutorial *tutorial);

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
