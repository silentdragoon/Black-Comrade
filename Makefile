
default: all

include Makefile.local

CC = g++
CFLAGS = -g -o2

OUTPUT = test

SRCS = \
main.cpp \
stateUpdate.cpp \
shipState.cpp \
accelerationState.cpp \
motionState.cpp \
inputState.cpp \
pilotControls.cpp \
gunState.cpp \
audioState.cpp \
replicaConnection.cpp \
replicaObject.cpp \
ourReplicaManager.cpp \
networkingManager.cpp \
discoveryAgent.cpp \
mapManager2.cpp \
rayQuery.cpp \
soundManager.cpp \
miniGameManager.cpp \
testMiniGame.cpp \
rect2D.cpp \
collaborationInfo.cpp \
lobby.cpp \
bullet.cpp \
bulletManager.cpp \
collisionDetection.cpp \
collisionManager.cpp \
collision.cpp \
gameStateMachine.cpp \
gameParameterMap.cpp \
waypoint.cpp \
printState.cpp \
enemy.cpp \
swarm.cpp \
swarmManager.cpp \
flying.cpp \
mapTile.cpp \
navigatorControls.cpp \
engineerControls.cpp \
lines.cpp \
DynamicLines.cpp \
DynamicRenderable.cpp \
sceneNodeManager.cpp \
guiManager.cpp \
guiStatusUpdater.cpp \
damageState.cpp \
constManager.cpp \
pathFinder.cpp \
pathTile.cpp \
notificationManager.cpp \
systemManager.cpp \
notification.cpp \
radarGui.cpp hud.cpp \
console.cpp consoleShell.cpp \
quickTimeMiniGame.cpp \
magicWordMiniGame.cpp \
buttonMashMiniGame.cpp \
CEGUIRQListener.cpp \
preGame.cpp \
particleSystemEffect.cpp \
particleSystemEffectManager.cpp \
playerStats.cpp \
gunnerControls.cpp \
networkRoleMenu.cpp \
gameRoleMenu.cpp \
loadingScreen.cpp \
postGame.cpp \
statsScreen.cpp \
gameEnder.cpp \
objective.cpp \
serverInfo.cpp \
sensorMiniGame.cpp \
fixMiniGame.cpp \
sequenceMiniGame.cpp \
sheildMiniGame.cpp \
storyMenu.cpp \
menuSystem.cpp \
fader.cpp \
door.cpp \
mapPieceChoices.cpp \
tutorial.cpp \
spotLight.cpp \
lightAndObjectsManager.cpp \
gameTimer.cpp \
screenshot.cpp

OBJS = $(SRCS:.cpp=.o)


all: $(OUTPUT)
	@echo Finished!
	
$(OUTPUT) : $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LFLAGS) $(LIBS)
	
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	
run:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; gdb -ex run ./$(OUTPUT)
	
nc:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; ./$(OUTPUT) -nc
	
debug:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; gdb ./$(OUTPUT)

kdebug:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; kdbg ./$(OUTPUT)

nodebug:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; ./$(OUTPUT)

.PHONY: clean depend

clean:
	rm *.o $(OUTPUT)
	
depend: $(SRCS)
	makedepend -f Makefile.local $^
# DO NOT DELETE
