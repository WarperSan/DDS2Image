cmake_minimum_required(VERSION 3.20)
project(dds2image)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include lib)

# Source files
set(SOURCES
        src/main.cpp
        src/Helpers/BinaryStream.cpp
        src/Readers/Reader.cpp
        src/Readers/DDS/DDSReader.cpp
        src/Readers/Factory.cpp
        lib/stb_image_write.h
)

add_executable(dds2image ${SOURCES})