set(PROJECT_NAME a.out)
set(IRRKLANG_DIR irrKlang-64bit-1.5.0/include)

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

find_package(OpenGL)
find_package(GLEW REQUIRED)
include_directories(${IRRKLANG_DIR})

if(APPLE)
   find_library(ASSIMP_LIB assimp)
   find_library(GLFW_LIB glfw3)
   find_library(COCOA_LIB Cocoa)
   find_library(IOKIT_LIB IOKit)
   find_library(COREVIDEO_LIB CoreVideo)
   find_library(IRRKLANG_LIB irrKlang)
   find_library(IRRKLANGMP3_LIB irrKlangMp3)
   set(GLFW_DEPENDENT_LIBS ${COCOA_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB})
   set(PLATFORM_LIBS ${GLFW_DEPENDENT_LIBS})

elseif(WIN32)
   set(GLFW_LIB glfw3)
   set(PLATFORM_LIBS glew32)

endif()

add_executable(${PROJECT_NAME} main.cpp GLSL.cpp tiny_obj_loader.cc Booth.cpp Camera.cpp MatrixStack.cpp Object.cpp Session.cpp SkyBox.cpp Window.cpp World.cpp Texture.cpp Clicks.cpp ShootingGallery.cpp Sound.cpp WatermelonSmash.cpp)

target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIB} ${GLEW_LIBRARY} ${PLATFORM_LIBS} ${IRRKLANG_LIB} ${IRRKLANGMP3_LIB})
