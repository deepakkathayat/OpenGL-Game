CC = g++
CFLAGS = -Wall
PROG = main

SRCS = main.cpp imageloader.cpp md2model.cpp vec3f.cpp camera.cpp text3d.cpp

LIBS = -lglut -lGLU -lGL

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
