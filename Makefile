#CC = g++
LOADLIBES = -lm
CFLAGS = -Wall -g


SRC1 = Agent.cpp Breeder.cpp CandidateSolution.cpp \
    Cupid.cpp FateAgent.cpp Grid.cpp Reaper.cpp \
    fitness.cpp

SRC2 = main.cpp #dirkOK.cpp benchmars.cpp benchmarksdeclare.cpp

SRC  = $(SRC1) $(SRC2)
OBJS = $(subst .cpp,.o, $(SRC))

AUX = $(subst .cpp,.h, $(SRC1))


main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(SRC) $(AUX)
	$(CC) $(CFLAGS) -c  $(SRC)
.PHONY: clean
clean:
	rm -f *.o main
