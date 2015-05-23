set(PROJECT_NAME a.out)
set(IRRKLANG_DIR irrKlang-64bit-1.5.0/include)
set(FREETYPE_DIR freetype-2.5.5/include)

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

find_package(OpenGL)
find_package(GLEW REQUIRED)
include_directories(${IRRKLANG_DIR})
include_directories(${FREETYPE_DIR})

if(APPLE)
   find_library(ASSIMP_LIB assimp)
   find_library(GLFW_LIB glfw3)
   find_library(COCOA_LIB Cocoa)
   find_library(IOKIT_LIB IOKit)
   find_library(COREVIDEO_LIB CoreVideo)
   find_library(IRRKLANG_LIB irrKlang)
   find_library(IRRKLANGMP3_LIB irrKlangMp3)
   find_library(FREETYPE_LIB freetype)
   set(GLFW_DEPENDENT_LIBS ${COCOA_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB})
   set(PLATFORM_LIBS ${GLFW_DEPENDENT_LIBS})

elseif(WIN32)
   set(GLFW_LIB glfw3)
   set(PLATFORM_LIBS glew32)

endif()

add_executable(${PROJECT_NAME} Booth.cpp Camera.cpp Clicks.cpp FontAtlas.cpp FontEngine.cpp GLSL.cpp main.cpp MatrixStack.cpp Minigame.cpp Object.cpp Particle.cpp ParticleTexture.cpp Program.cpp Session.cpp ShootingGallery.cpp Skybox.cpp Sound.cpp Texture.cpp tiny_obj_loader.cc WatermelonSmash.cpp Window.cpp World.cpp)

target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIB} ${GLEW_LIBRARY} ${PLATFORM_LIBS} ${IRRKLANG_LIB} ${IRRKLANGMP3_LIB} ${FREETYPE_LIB})
