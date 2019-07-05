cmake_minimum_required(VERSION 3.14)
project(Z2)

set(CMAKE_CXX_STANDARD 14)
file(GLOB Core src/core/*.cpp src/core/*.h src/core/*/*.cpp src/core/*/*.h)
file(GLOB World src/world/*.cpp src/world/*.h)
file(GLOB Entity src/entity/*.cpp src/entity/*.h)
file(GLOB Player src/player/*.cpp src/player/*.h)
file(GLOB Event src/event/*.cpp src/event/*.h)
file(GLOB Util src/util/*.cpp src/util/*.h)
file(GLOB CommandLine src/commandLine/*.cpp src/commandLine/*.h)
add_executable(z2
        src/main.cpp
        ${Core} ${World} ${Entity} ${Player} ${Event} ${Util}
        ${CommandLine}
        )