CC=g++
OPTS=-g -std=c++11 -Werror

all: main.o predictor.o
	$(CC) $(OPTS) -lm -o predictor main.o predictor.o

main.o: main.cpp predictor.hpp
	$(CC) $(OPTS) -c main.cpp

predictor.o: predictor.hpp predictor.cpp
	$(CC) $(OPTS) -c predictor.cpp

test-static: all
	bunzip2 -kc ../traces/${trace}.bz2 | ./predictor --static

test-gshare: all
	bunzip2 -kc ../traces/${trace}.bz2 | ./predictor --gshare:20

test-tournament: all
	bunzip2 -kc ../traces/${trace}.bz2 | ./predictor --tournament:20:20:20

test-custom: all
	bunzip2 -kc ../traces/${trace}.bz2 | ./predictor --custom

clean:
	rm -f *.o predictor;
