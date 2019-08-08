cmake_minimum_required(VERSION 3.14)
project(Z2)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)
include_directories(lib src)


file(GLOB Core src/core/*.cpp src/core/*.h src/core/*/*.cpp src/core/*/*.h)
file(GLOB World src/world/*.cpp src/world/*.h)
file(GLOB Entity src/entity/*.cpp src/entity/*.h)
file(GLOB Event src/event/*.cpp src/event/*.h)
file(GLOB Config src/config/*.cpp src/config/*.h)
file(GLOB Bot src/bot/*.cpp src/bot/*.h)
file(GLOB CommandLine src/commandLine/*.cpp src/commandLine/*.h)
file(GLOB Util src/util/*.cpp src/util/*.h)
file(GLOB_RECURSE Lib lib lib/*.hpp lib/*.cc lib/*.h lib/*.ipp)
#aux_source_directory(lib Lib)
set(Z2_Sources ${Core} ${World} ${Entity} ${Event} ${Config} ${Bot} ${CommandLine} ${Util} ${Lib})
#add_executable(z2
#        src/main.cpp ${Z2_Sources}
#        )
#add_executable(Client
#        src/ClientMain.cpp
#        ${Z2_Sources}
#        )
#add_executable(Server
#        src/ServerMain.cpp
#        ${Z2_Sources}
#        )
add_library(Z2Core ${Z2_Sources})
install(TARGETS Z2Core DESTINATION ${PROJECT_SOURCE_DIR}/../api/z2Core)
install(DIRECTORY src/ lib/ DESTINATION ${PROJECT_SOURCE_DIR}/../api/z2Core/include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.ipp"
        PATTERN "*.hpp")



