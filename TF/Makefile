CC = g++
LIBS = -lGL -lglut -lGLU

EXE = trabalhocg
PROGRAM = trabsson.cpp
TINY = tiny/*.cpp
CLASSES = application/*.cpp
MESH = mesh/*.cpp

all:
	$(CC) -o $(EXE) $(PROGRAM) $(CLASSES) $(TINY) $(MESH) $(LIBS)

run:
	./trabalhocg TF/

clean:
	rm -rf $(EXE); rm -rf tiny/*.o; rm -rf application/*.o
