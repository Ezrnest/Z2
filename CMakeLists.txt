cmake_minimum_required(VERSION 3.14)
project(Z2)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)
include_directories(lib src)


file(GLOB Core src/core/*.cpp src/core/*.h src/core/*/*.cpp src/core/*/*.h)
file(GLOB World src/world/*.cpp src/world/*.h)
file(GLOB Entity src/entity/*.cpp src/entity/*.h)
file(GLOB Player src/player/*.cpp src/player/*.h)
file(GLOB Event src/event/*.cpp src/event/*.h)
file(GLOB Util src/util/*.cpp src/util/*.h)
file(GLOB CommandLine src/commandLine/*.cpp src/commandLine/*.h)
file(GLOB_RECURSE Lib lib *.hpp *.cc *.h *.ipp)
#aux_source_directory(lib Lib)
add_executable(z2
        src/main.cpp
        ${Core} ${Lib} ${World} ${Entity} ${Player} ${Event} ${Util}
        ${CommandLine}
        )