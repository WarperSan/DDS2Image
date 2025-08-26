# DDS2Image

A simple C++ command-line tool to convert DDS files to common image formats.

## Usage

```bash
dds2image <input.*> <output.*>
```

Depending on the readers/writers implemented, different file types will be supported.

## Build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```