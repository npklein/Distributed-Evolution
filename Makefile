CC = g++
LOADLIBES = -lm
<<<<<<< HEAD
CFLAGS = -Wall -O2 -g
=======
CFLAGS = -Wall -g
>>>>>>> master


SRC1 = Agent.cpp Breeder.cpp CandidateSolution.cpp \
    Cupid.cpp FateAgent.cpp Grid.cpp Reaper.cpp \
    fitness.cpp \
    benchmarks.cpp \
    benchmarkshelper.cpp benchmarksnoisy.cpp benchmarksdeclare.cpp

<<<<<<< HEAD
SRC2 = main.cpp

SRC  = $(SRC1) $(SRC2)

OBJS = $(SRC1:.cpp = .o)

AUX = $(SRC1:.c = .h)


main: $(OBJS) 
#   $(CC) $(CFLAGS) -o $(SRC) $(AUX) 

=======
SRC2 = main.cpp fgeneric.cpp  dirOK.cpp #benchmars.cpp benchmarksdeclare.cpp

SRC  = $(SRC1) $(SRC2)
OBJS = $(subst .cpp,.o, $(SRC))

AUX = $(subst .cpp,.h, $(SRC1)) bbobStructures.h benchmarks.h


main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(SRC) $(AUX)
	$(CC) $(CFLAGS) -c  $(SRC)
>>>>>>> master
.PHONY: clean
clean:
	rm -f *.o main
