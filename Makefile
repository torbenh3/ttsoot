

CXXFLAGS=-O3 -fargument-noalias-anything -funsafe-math-optimizations `pkg-config --cflags jack` --std=gnu++0x
LDFLAGS=`pkg-config --libs jack`
CXX=g++-4.4

OBJECTS=block.o main.o process.o parammap.o

all: ttsoot process.s

ttsoot: $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

%.o:%.cc
	$(CXX) -MD -MF .deps/$(@:.o=.d) -c $(CXXFLAGS) -o $@ $<

%.s:%.cc
	$(CXX) -S $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm *.o *.s *.cc.*

-include .deps/*.d
