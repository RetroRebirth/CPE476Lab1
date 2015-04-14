CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp
LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)
