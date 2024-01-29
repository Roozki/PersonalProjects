cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
#light system
project(light_system_esp32)


target_include_directories(app PRIVATE 
    include
    src
)

#telling it what targets to aim for
FILE(GLOB app_sources src/*.cpp)
target_sources(app PRIVATE ${app_sources})
