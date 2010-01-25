
default: all

include Makefile.local

CC = g++ -g
CFLAGS = 

OUTPUT = test

SRCS = main.cpp stateUpdate.cpp shipState.cpp accelerationState.cpp motionState.cpp keyState.cpp shipControls.cpp frontGunState.cpp audioState.cpp ReplicaConnection.cpp ReplicaObject.cpp OurReplicaManager.cpp networkingManager.cpp DiscoveryAgent.cpp mapCreate.cpp enemyState.cpp rayQuery.cpp soundManager.cpp miniGameManager.cpp testMiniGame.cpp rect2D.cpp collaborationInfo.cpp lobby.cpp bullet.cpp bulletManager.cpp collisionDetection.cpp collisionManager.cpp collision.cpp gameStateMachine.cpp gameParameterMap.cpp waypoint.cpp printState.cpp

OBJS = $(SRCS:.cpp=.o)


all: $(OUTPUT)
	@echo Finished!
	
$(OUTPUT) : $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LFLAGS) $(LIBS)
	
.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)
	
run:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; ./$(OUTPUT)
	
debug:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; gdb ./$(OUTPUT)

.PHONY: clean depend

clean:
	rm *.o $(OUTPUT)
	
depend: $(SRCS)
	makedepend -f Makefile.local $^
# DO NOT DELETE
