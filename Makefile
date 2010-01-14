

CXXFLAGS=-O3 -ffast-math -fgcse-sm -march=core2 -funsafe-math-optimizations `pkg-config --cflags jack gtkmm-2.4` -std=gnu++0x -msse3 

# --param inline-unit-growth=100000 --param large-function-growth=100000 --param large-stack-frame-growth=100000 --param max-inline-insns-single=1000000
LDFLAGS=`pkg-config --libs jack gtkmm-2.4`
CXX=/opt/gcc/bin/g++

OBJECTS=main.o process.o parammap.o gui.o gui_adjustment.o oscilators.o basicblocks.o filters.o block.o

all: ttsoot process.s

ttsoot: $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

%.o:%.cc
	$(CXX) -MD -MF .deps/$(@:.o=.d) -c $(CXXFLAGS) -o $@ $<

%.s:%.cc
	$(CXX) -S $(CXXFLAGS) -fdump-tree-all -o $@ $<

.PHONY: clean
clean:
	rm *.o *.s *.cc.*

-include .deps/*.d
