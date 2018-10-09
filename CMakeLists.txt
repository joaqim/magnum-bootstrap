cmake_minimum_required(VERSION 2.8.12)
project(MyApplication)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(OpenGL_GL_PREFERENCE "LEGACY")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules/")

add_subdirectory(src)
