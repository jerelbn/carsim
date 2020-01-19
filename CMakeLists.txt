cmake_minimum_required (VERSION 3.5.1)
project (carsim)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type: Release")
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(yaml-cpp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_library(common_cpp INTERFACE)
 
include_directories(include
    lib/common_cpp/include
    ${YAML_CPP_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

add_executable(carsim
    src/main.cpp
    src/glanimator.cpp
    src/bicycle.cpp
)
target_link_libraries(carsim
    common_cpp
    ${YAML_CPP_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

