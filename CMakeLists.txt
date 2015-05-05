set(PROJECT_NAME a.out)

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

find_package(OpenGL)
find_package(GLEW REQUIRED)

if(APPLE)
   find_library(ASSIMP_LIB assimp)
   find_library(GLFW_LIB glfw3)
   find_library(COCOA_LIB Cocoa)
   find_library(IOKIT_LIB IOKit)
   find_library(COREVIDEO_LIB CoreVideo)
   set(GLFW_DEPENDENT_LIBS ${COCOA_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB})
   set(PLATFORM_LIBS ${GLFW_DEPENDENT_LIBS})

elseif(WIN32)
   set(GLFW_LIB glfw3)
   set(PLATFORM_LIBS glew32)

endif()

add_executable(${PROJECT_NAME} main.cpp GLSL.cpp tiny_obj_loader.cc Booth.cpp Camera.cpp MatrixStack.cpp Object.cpp Player.cpp Session.cpp SkyBox.cpp Window.cpp World.cpp Texture.cpp)

target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIB} ${GLEW_LIBRARY} ${PLATFORM_LIBS})
