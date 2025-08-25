# DDS2Image
A simple C++ command-line tool to convert DDS files to common image formats.

## Usage
```bash
dds2image <input.dds> <output.*>
```

## Build
```bash
 g++ -O2 -std=c++2a src/main.cpp src/Converter.cpp src/BinaryReader.cpp src/Factory.cpp  -flto  -o build/dds2image
```