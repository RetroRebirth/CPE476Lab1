export IRRKLANG_INC := irrKlang-64bit-1.5.0/include
export IRRKLANG_LIB_U := irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so
export IRRKLANG_MP3_U := irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpMP3.so

CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS= -I $(ROOT_DIR) -I $(FREETYPE_DIR) -I $(IRRKLANG_INC) -L $(ROOT_DIR) -g
CSL_CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp Sound.cpp
CSL_SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp
LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut -lassimp -lfreetype $(IRRKLANG_LIB_U) $(IRRKLANG_MP3_U) libglfw3.a libGLEW.a
CSL_LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a

all:
	cp includesmac.h includes.h
	$(CC) $(CFLAsGS) $(SOURCES) $(LIBFLAGS)
	
ubuntu:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)
	
ubuntu:
	cp includes_ubuntu.h includes.h
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)

csl:
	cp includes_csl.h includes.h
	$(CC) $(CSL_CFLAGS) $(CSL_SOURCES) $(CSL_LIBFLAGS)

rebase:
	git add . 
	git commit -m "$(msg)"
	git fetch
	git rebase origin/master
