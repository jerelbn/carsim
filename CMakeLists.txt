cmake_minimum_required (VERSION 3.5.1)
project (carsim)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type: Release")
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(lib/common_cpp)
 
include_directories(include
    lib/common_cpp/include
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

add_executable(SimpleAnim src/SimpleAnim.cpp)
target_link_libraries(SimpleAnim
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

