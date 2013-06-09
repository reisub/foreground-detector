PROJECT			= detector

CC				= g++
CFLAGS			= -c -g -fopenmp -Wall -Wextra
LDFLAGS			= -lgomp

SOURCES  		= detector.cpp foreground.cpp background.cpp gaussian.cpp
OBJECTS 		= ${SOURCES:.cpp=.o}

EXECUTABLE		= detector

CFLAGS			+= `pkg-config --cflags opencv`
LDFLAGS			+= `pkg-config --libs opencv`


all: $(SOURCES) $(EXECUTABLE)

gauss: gaussian.hpp gaussian.cpp testGauss.cpp
	$(CC) $(CFLAGS) gaussian.cpp -o gaussian.o
	$(CC) $(CFLAGS) testGauss.cpp -o testGauss.o
	$(CC) gaussian.o testGauss.o -o gauss $(LDFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
