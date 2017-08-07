CC = g++
CFLAGS = --std=c++1z -Wall -Werror -Wextra -Wpedantic -Wno-unused-parameter
OBJS = Cards.o ChoiceAdapter.o GameMaster.o GodBook.o State.o Player.o ActionDetails.o PlanetDetails.o
HDRS = CoreEnums.h util/Enum.h

all: cli test debug

cli: $(OBJS)
	$(CC) $(CFLAGS) -O3 $(OBJS) main.cc -o ../bin/ed-cli

test: $(OBJS)
	$(CC) $(CFLAGS) -O0 -g $(OBJS) tests.cc -o ../bin/test-suite

debug: $(OBJS)
	$(CC) $(CFLAGS) -O0 -g $(OBJS) main.cc -o ../bin/ed-cli.debug

Cards.o: Cards.h CardDetails.h Cards.cc ActionDetails.cc PlanetDetails.cc GodBook.cc $(HDRS)
	$(CC) $(CFLAGS) -c Cards.cc ActionDetails.cc PlanetDetails.cc GodBook.cc

ChoiceAdapter.o: ChoiceAdapter.h ChoiceAdapter.cc $(HDRS)
	$(CC) $(CFLAGS) -c ChoiceAdapter.cc

GameMaster.o: GameMaster.h GameMaster.cc $(HRDS)
	$(CC) $(CFLAGS) -c GameMaster.cc

GodBook.o: GodBook.h GodBook.cc
	$(CC) $(CFLAGS) -c GodBook.cc

State.o: State.h State.cc $(HDRS)
	$(CC) $(CFLAGS) -c State.cc

Player.o: Player.h Player.cc $(HDRS)
	$(CC) $(CFLAGS) -c Player.cc

clean:
	rm *.o
