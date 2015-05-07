CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS=  -I $(ROOT_DIR) -I $(FREETYPE_DIR) -L $(ROOT_DIR) -g
CSL_CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Model.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp Sound.cpp
CSL_SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp
LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut -lassimp -lfreetype $(IRRKLANGMP3_DIR) $(IRRKLANG_DIR) $(IRRKLANGFLAC_DIR) libglfw3.a libGLEW.a
CSL_LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a

all:
	cp includes_mac.h includes.h
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
