all: roc

roc: roc.o
	g++ -std=c++11 -g roc.o
	mv a.out roc

roc.o: main.cpp
	g++ -std=c++11 -c main.cpp
	mv main.o roc.o
