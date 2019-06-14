cmake_minimum_required(VERSION 3.12)
project(untitled)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c client.c Sprite.c LinkedList.c Sprite.h LinkedList.h Settings.h threadTest.c)