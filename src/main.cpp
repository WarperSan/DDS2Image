#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "../include/BinaryReader.h"
#include "../include/Converter.h"
#include "../include/Factory.h"

std::vector<uint8_t> readFromFile(std::string file)
{
    std::ifstream steam(file, std::ios::binary | std::ios::ate);

    if (!steam)
        throw std::runtime_error("Failed to open file.");

    std::streamsize size = steam.tellg();
    steam.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!steam.read(reinterpret_cast<char *>(buffer.data()), size))
        throw std::runtime_error("Failed to read file.");

    return buffer;
}

Result convertBuffer(std::vector<uint8_t> buffer)
{
    std::unique_ptr<Converter> converter = Factory::create(buffer);

    return converter.get()->process();
}

int main(int argc, char *argv[])
{
    // Read file to buffer
    std::vector<uint8_t> buffer = readFromFile("test.dds");

    // Convert buffer
    const Result result = convertBuffer(buffer);

    // Output
    stbi_write_png("output.png", result.width, result.height, 4, result.data.data(), result.width);

    return 0;
}