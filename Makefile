

CXXFLAGS=-g -O3 -fargument-noalias-anything

all: block.o main.o main.s

%.s : %.cc
	gcc -S $(CXXFLAGS) -o $@ $<
