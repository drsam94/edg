all: main.cc *.h *.cc
	g++ -o ../bin/ed-cli *.cc -O3 --std=c++1z -Wall -Werror -Wextra -Wpedantic -Wno-unused-parameter

debug: main.cc *.h *.cc
	g++ -o ../bin/ed-cli.debug *.cc -O0 -g --std=c++1z -Wall -Werror -Wextra -Wpedantic
