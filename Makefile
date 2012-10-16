CC = g++
LOADLIBES = -lm
CFLAGS = -Wall -O2 -g


SRC1 = Agent.cpp Breeder.cpp CandidateSolution.cpp \
    Cupid.cpp FateAgent.cpp Grid.cpp Reaper.cpp \
    fitness.cpp

SRC2 = main.cpp

SRC  = $(SRC1) $(SRC2)

OBJS = $(SRC1:.cpp = .o)

AUX = $(SRC1:.c = .h)


main: $(OBJS) 
#   $(CC) $(CFLAGS) -o $(SRC) $(AUX) 

.PHONY: clean
clean:
	rm -f *.o main