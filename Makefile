
default: all

include Makefile.local

CC = g++
CFLAGS = 

OUTPUT = test

SRCS = main.cpp stateUpdate.cpp shipState.cpp engineState.cpp motionState.cpp

OBJS = $(SRCS:.cpp=.o)


all: $(OUTPUT)
	@echo Finished!
	
$(OUTPUT) : $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LFLAGS) $(LIBS)
	
.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)
	
run:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/"; ./$(OUTPUT)
	
.PHONY: clean depend

clean:
	rm *.o $(OUTPUT)
	
depend: $(SRCS)
	makedepend -f Makefile.local $^
