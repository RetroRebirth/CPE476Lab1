export IRRKLANG_INC := irrKlang-1.5.0/include
export IRRKLANG_LIB_U := irrKlang-1.5.0/bin/linux-gcc/libIrrKlang.so
export IRRKLANG_MP3_U := irrKlang-1.5.0/bin/linux-gcc/ikpMP3.so

CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS=  -I $(ROOT_DIR) -I$(FREETYPE_DIR) -I $(IRRKLANG_INC) -L $(ROOT_DIR) -g
CSL_CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp Sound.cpp WatermelonSmash.cpp Minigame.cpp Particle.cpp ParticleTexture.cpp Program.cpp FontEngine.cpp FontAtlas.cpp Karaoke.cpp
CSL_SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp WatermelonSmash.cpp Minigame.cpp Particle.cpp ParticleTexture.cpp Program.cpp Karaoke.cpp
LIBFLAGS= -std=c++0x -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lfreetype $(IRRKLANG_LIB_U) $(IRRKLANG_MP3_U) libglfw3.a libGLEW.a
CSL_LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a
 
ubuntu:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)

chris:
	$(CC) $(CFLAGS) -std=c++0x `pkg-config --cflags glfw3` -o myprog $(SOURCES) -lfreetype $(IRRKLANG_LIB_U) $(IRRKLANG_MP3_U) `pkg-config --static --libs glfw3` -lGLEW

rebase:
	git add . 
	git commit -m "$(msg)"
	git fetch
	git rebase origin/master
