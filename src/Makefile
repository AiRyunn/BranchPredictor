CC=g++
OPTS=-g -std=c++11 -Werror

all: main.o predictor.o gshare.o bimodel.o tournament.o perceptron.o
	$(CC) $(OPTS) -lm -o predictor main.o predictor.o gshare.o bimodel.o tournament.o perceptron.o

main.o: main.cpp predictor.hpp
	$(CC) $(OPTS) -c main.cpp

predictor.o: predictor.hpp predictor.cpp
	$(CC) $(OPTS) -c predictor.cpp

gshare.o: gshare.hpp gshare.cpp
	$(CC) $(OPTS) -c gshare.cpp

bimodel.o: bimodel.hpp bimodel.cpp
	$(CC) $(OPTS) -c bimodel.cpp

tournament.o: tournament.hpp tournament.cpp
	$(CC) $(OPTS) -c tournament.cpp

perceptron.o: perceptron.hpp perceptron.cpp
	$(CC) $(OPTS) -c perceptron.cpp

test: all
	bunzip2 -kc ../traces/${trace}.bz2 | ./predictor --${predictor}

clean:
	rm -f *.o predictor;
