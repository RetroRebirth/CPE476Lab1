export IRRKLANG_INC := irrKlang-1.5.0/include
export IRRKLANG_LIB_U := irrKlang-1.5.0/bin/linux-gcc/libIrrKlang.so
export IRRKLANG_MP3_U := irrKlang-1.5.0/bin/linux-gcc/ikpMP3.so

export IRRKLANG_INC_x64 := irrKlang-64-bit-1.5.0/include
export IRRKLANG_LIB_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so
export IRRKLANG_MP3_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpMP3.so

CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS=  -I $(ROOT_DIR) -I$(FREETYPE_DIR) -I $(IRRKLANG_INC) -L $(ROOT_DIR) -g -std=c++0x `pkg-config --cflags opencv`
CSL_CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.o GLSL.o tiny_obj_loader.cc Camera.o Object.o Window.o World.o MatrixStack.o Booth.o SkyBox.o Session.o Texture.o Clicks.o ShootingGallery.o Sound.o WatermelonSmash.o Minigame.o Particle.o ParticleTexture.o Program.o FontEngine.o FontAtlas.o Karaoke.o
CSL_SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp WatermelonSmash.cpp Minigame.cpp Particle.cpp ParticleTexture.cpp Program.cpp Karaoke.cpp
LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lfreetype `pkg-config --libs opencv` $(IRRKLANG_LIB_U_x64) $(IRRKLANG_MP3_U_x64)  libGLEW.a libglfw3.a
CSL_LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a
 
ubuntu: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)

chris: $(SOURCES)
	cp libglfw3_x32.a libglfw3.a
	cp libGLEW_x32.a libGLEW.a
	$(CC) $(CFLAGS) -std=c++0x `pkg-config --cflags glfw3` -o myprog $(SOURCES) -lfreetype $(IRRKLANG_LIB_U) $(IRRKLANG_MP3_U) `pkg-config --static --libs glfw3` -lGLEW

rebase:
	git add . 
	git commit -m "$(msg)"
	git fetch
	git rebase origin/master

# optimize makefile by only compiling what was changed
main.o : main.cpp
	g++ $(CFLAGS) -c main.cpp

GLSL.o : GLSL.cpp
	g++ $(CFLAGS) -c GLSL.cpp

Camera.o : Camera.cpp
	g++ $(CFLAGS) -c Camera.cpp

Object.o : Object.cpp
	g++ $(CFLAGS) -c Object.cpp

Window.o : Window.cpp
	g++ $(CFLAGS) -c Window.cpp

World.o : World.cpp
	g++ $(CFLAGS) -c World.cpp

MatrixStack.o : MatrixStack.cpp
	g++ $(CFLAGS) -c MatrixStack.cpp

Booth.o : Booth.cpp
	g++ $(CFLAGS) -c Booth.cpp

SkyBox.o : SkyBox.cpp
	g++ $(CFLAGS) -c SkyBox.cpp

Session.o : Session.cpp
	g++ $(CFLAGS) -c Session.cpp

Texture.o : Texture.cpp
	g++ $(CFLAGS) -c Texture.cpp

Clicks.o : Clicks.cpp
	g++ $(CFLAGS) -c Clicks.cpp

ShootingGallery.o : ShootingGallery.cpp
	g++ $(CFLAGS) -c ShootingGallery.cpp

Sound.o : Sound.cpp
	g++ $(CFLAGS) -c Sound.cpp

WatermelonSmash.o : WatermelonSmash.cpp
	g++ $(CFLAGS) -c WatermelonSmash.cpp

Minigame.o : Minigame.cpp
	g++ $(CFLAGS) -c Minigame.cpp

Particle.o : Particle.cpp
	g++ $(CFLAGS) -c Particle.cpp

ParticleTexture.o : ParticleTexture.cpp
	g++ $(CFLAGS) -c ParticleTexture.cpp

Program.o : Program.cpp
	g++ $(CFLAGS) -c Program.cpp

FontEngine.o : FontEngine.cpp
	g++ $(CFLAGS) -c FontEngine.cpp

FontAtlas.o : FontAtlas.cpp
	g++ $(CFLAGS) -c FontAtlas.cpp

Karaoke.o : Karaoke.cpp
	g++ $(CFLAGS) -c Karaoke.cpp
