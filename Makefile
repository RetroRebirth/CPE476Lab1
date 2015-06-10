export IRRKLANG_INC := irrKlang-1.5.0/include
export IRRKLANG_LIB_U := irrKlang-1.5.0/bin/linux-gcc/libIrrKlang.so
export IRRKLANG_MP3_U := irrKlang-1.5.0/bin/linux-gcc/ikpMP3.so

export IRRKLANG_INC_x64 := irrKlang-64-bit-1.5.0/include
export IRRKLANG_LIB_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so
export IRRKLANG_MP3_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpMP3.so

CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS=  -I $(ROOT_DIR) -I$(FREETYPE_DIR) -I $(IRRKLANG_INC) -L $(ROOT_DIR) -g -std=c++0x `pkg-config --cflags opencv`
#CSL_CFLAGS=  -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= main.o GLSL.o tiny_obj_loader.o Camera.o Object.o Window.o World.o MatrixStack.o Booth.o SkyBox.o Session.o Texture.o Clicks.o ShootingGallery.o Sound.o WatermelonSmash.o Minigame.o Particle.o ParticleTexture.o Program.o FontEngine.o FontAtlas.o Karaoke.o Shop.o
#CSL_SOURCES= main.cpp GLSL.cpp tiny_obj_loader.cc Camera.cpp Object.cpp Window.cpp World.cpp MatrixStack.cpp Booth.cpp SkyBox.cpp Session.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp WatermelonSmash.cpp Minigame.cpp Particle.cpp ParticleTexture.cpp Program.cpp Karaoke.cpp Shop.cpp
LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lfreetype `pkg-config --libs opencv` $(IRRKLANG_LIB_U_x64) $(IRRKLANG_MP3_U_x64)  libGLEW.a libglfw3.a
CSL_LIBFLAGS= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglut libglfw3.a libGLEW.a
 
ubuntu: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) $(LIBFLAGS)

chris: $(SOURCES)
	cp libglfw3_x32.a libglfw3.a
	cp libGLEW_x32.a libGLEW.a
	$(CC) $(CFLAGS) -std=c++0x `pkg-config --cflags glfw3` -o myprog $(SOURCES) -lfreetype $(IRRKLANG_LIB_U) $(IRRKLANG_MP3_U) `pkg-config --static --libs glfw3 opencv` -lGLEW

rebase:
	git add . 
	git commit -m "$(msg)"
	git fetch
	git rebase origin/master

clean:
	rm $(SOURCES)

# optimize makefile by only compiling what was changed
main.o : main.cpp includes.h Session.h Window.h Camera.h Texture.h Karaoke.h
	g++ $(CFLAGS) -c main.cpp

tiny_obj_loader.o : tiny_obj_loader.cc tiny_obj_loader.h
	g++ $(CFLAGS) -c tiny_obj_loader.cc

GLSL.o : GLSL.cpp glew.h GLSL.h
	g++ $(CFLAGS) -c GLSL.cpp

Camera.o : Camera.cpp Camera.h includes.h Window.h Object.h Booth.h
	g++ $(CFLAGS) -c Camera.cpp

Object.o : Object.cpp Object.h includes.h Texture.h
	g++ $(CFLAGS) -c Object.cpp

Window.o : Window.cpp Window.h includes.h
	g++ $(CFLAGS) -c Window.cpp

World.o : World.cpp World.h includes.h Object.h Window.h Camera.h SkyBox.h Booth.h Particle.h
	g++ $(CFLAGS) -c World.cpp

MatrixStack.o : MatrixStack.cpp glm/glm.hpp glm/gtc/type_ptr.hpp
	g++ $(CFLAGS) -c MatrixStack.cpp

Booth.o : Booth.cpp Booth.h includes.h Object.h Texture.h FontEngine.h
	g++ $(CFLAGS) -c Booth.cpp

SkyBox.o : SkyBox.cpp SkyBox.h includes.h Camera.h Object.h Texture.h
	g++ $(CFLAGS) -c SkyBox.cpp

Session.o : Session.cpp Session.h includes.h Window.h Camera.h World.h Clicks.h Sound.h Minigame.h Program.h ParticleTexture.h FontEngine.h
	g++ $(CFLAGS) -c Session.cpp

Texture.o : Texture.cpp Texture.h includes.h
	g++ $(CFLAGS) -c Texture.cpp

Clicks.o : Clicks.cpp Clicks.h includes.h Object.h
	g++ $(CFLAGS) -c Clicks.cpp

ShootingGallery.o : ShootingGallery.cpp ShootingGallery.h includes.h Clicks.h Texture.h Window.h Sound.h FontEngine.h
	g++ $(CFLAGS) -c ShootingGallery.cpp

Sound.o : Sound.cpp Sound.h includes.h Texture.h Camera.h
	g++ $(CFLAGS) -c Sound.cpp

WatermelonSmash.o : WatermelonSmash.cpp WatermelonSmash.h includes.h Clicks.h Texture.h Window.h Sound.h Program.h FontEngine.h Particle.h Camera.h ParticleSorter.h
	g++ $(CFLAGS) -c WatermelonSmash.cpp

Minigame.o : Minigame.cpp Minigame.h includes.h Window.h ShootingGallery.h WatermelonSmash.h Karaoke.h Shop.h
	g++ $(CFLAGS) -c Minigame.cpp

Particle.o : Particle.cpp Particle.h includes.h Program.h
	g++ $(CFLAGS) -c Particle.cpp

ParticleTexture.o : ParticleTexture.cpp ParticleTexture.h includes.h
	g++ $(CFLAGS) -c ParticleTexture.cpp

Program.o : Program.cpp Program.h includes.h ParticleTexture.h
	g++ $(CFLAGS) -c Program.cpp

FontEngine.o : FontEngine.cpp FontEngine.h includes.h FontAtlas.h Program.h
	g++ $(CFLAGS) -c FontEngine.cpp

FontAtlas.o : FontAtlas.cpp FontAtlas.h includes.h
	g++ $(CFLAGS) -c FontAtlas.cpp

Karaoke.o : Karaoke.cpp Karaoke.h includes.h Clicks.h Texture.h Window.h Sound.h FontEngine.h Program.h Particle.h Camera.h
	g++ $(CFLAGS) -c Karaoke.cpp

Shop.o : Shop.cpp Shop.h includes.h Clicks.h Texture.h Window.h Sound.h FontEngine.h
	g++ $(CFLAGS) -c Shop.cpp
