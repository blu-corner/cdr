INSTALL_PREFIX=../../build/install
CXX=g++

CFLAGS=-I$(INSTALL_PREFIX)/include/cdr/
LDFLAGS=-L$(INSTALL_PREFIX)/lib 
LIBS=-lcdr

all: example

example: example.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

example.o: example.cpp
	$(CXX) -c $(CFLAGS) $<

clean:
	rm *.o example
