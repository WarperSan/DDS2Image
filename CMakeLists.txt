cmake_minimum_required(VERSION 4.0)
project(dds2image)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dds2image
        src/main.cpp
        lib/stb_image_write.h
        include/Helpers/BinaryStream.h
        src/Helpers/BinaryStream.cpp
        src/Readers/Reader.cpp
        include/Readers/Reader.h
        src/Readers/DDS/DDSReader.cpp
        include/Readers/DDS/DDSReader.h
        src/Readers/Factory.cpp
        include/Readers/Factory.h
        include/Readers/DDS/ATCIReader.h
        include/Readers/DDS/ATCAReader.h
)
