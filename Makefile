#CC = g++
LOADLIBES = -lm
CXXFLAGS = -Wall -g


SRC1 = Agent.cpp Breeder.cpp CandidateSolution.cpp \
    Cupid.cpp FateAgent.cpp Grid.cpp Reaper.cpp \
    fitness.cpp

SRC2 = main.cpp

SRC  = $(SRC1) $(SRC2)

OBJS = $(SRC1:.cpp=.o)

AUX = $(SRC1:.c=.h)

main: $(OBJS) $(SRC2)
#   Built by implicit rules

.PHONY: clean
clean:
	rm -f *.o main