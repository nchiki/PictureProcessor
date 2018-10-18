cmake_minimum_required(VERSION 3.10)
project(cpp_pic_proc_nc1517)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(cpp_pic_proc_nc1517
        Colour.cpp
        Colour.hpp
        Main.cpp
        PicLibrary.cpp
        PicLibrary.hpp
        Picture.cpp
        Picture.hpp
        Utils.cpp
        Utils.hpp PicWrapper.cpp PicWrapper.h)
