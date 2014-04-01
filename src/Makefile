SHELL		= /bin/sh
EXE 		= fighter
BUILDDIR	= ../bin
SRCDIR		= src/
OBJS		= math3d.o readfile.o camera.o dobj.o spherebounded.o game.o main.o
COMP 		= g++
LINK		= -lGL -lglut -lGLEW
MAIN_OBJS 	= main.h game.h main.cpp
GAME_OBJS	= game.h camera.h math3d.h game.cpp
CAMERA_OBJS = camera.h math3d.h camera.cpp
MATH_OBJS 	= math3d.h math3d.cpp
SPHEREB_OBJS= spherebounded.h dobj.h readfile.h spherebounded.cpp
DOBJ_OBJS 	= dobj.h math3d.h dobj.cpp 
READ_OBJS 	= readfile.cpp readfile.h
COMP_OPTS 	= -c -g -O0 -Werror


all: $(EXE)

$(EXE): $(OBJS)
	mkdir -p $(BUILDDIR)
	$(COMP) $(LINK) $(OBJS) -o $(BUILDDIR)/$(EXE)
	cp -r ../include/* $(BUILDDIR)

main.o: $(MAIN_OBJS)
	$(COMP) $(COMP_OPTS) $(MAIN_OBJS) 

game.o: $(GAME_OBJS)
	$(COMP) $(COMP_OPTS) $(GAME_OBJS) 

camera.o: $(CAMERA_OBJS)
	$(COMP) $(COMP_OPTS) $(CAMERA_OBJS) 

math3d.o: $(MATH_OBJS)
	$(COMP) $(COMP_OPTS) $(MATH_OBJS) 

spherebounded.o: $(SPHEREB_OBJS)
	$(COMP) $(COMP_OPTS) $(SPHEREB_OBJS) 

dobj.o: $(DOBJ_OBJS)
	$(COMP) $(COMP_OPTS) $(DOBJ_OBJS) 

readfile.o: $(READ_OBJS)
	$(COMP) $(COMP_OPTS) $(READ_OBJS)

clean:
	-rm -f *.o *.h.gch 
	-rm -f  -r $(BUILDDIR)/*