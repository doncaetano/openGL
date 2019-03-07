all: tiny ap
	g++ -Itinyxml -Iapplication trabsson.cpp tinyxml/tinyxml.o tinyxml/tinystr.o tinyxml/tinyxmlerror.o tinyxml/tinyxmlparser.o application/application.o application/circle.o application/window.o -o trabalhocg -lGL -lGLU -lglut -lm

tiny:
	+$(MAKE) -C tinyxml

ap:
	+$(MAKE) -C application

clean: 
	rm -rf tinyxml/*.o; rm -rf trabalhocg;rm -rf application/*.o