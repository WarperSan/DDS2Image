#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include "../include/Converters/Converter.h"
#include "../include/Converters/ConverterFactory.h"

#include "../include/Encoders/Encoder.h"
#include "../include/Encoders/PNGEncoder.h"
#include "../include/Encoders/AnimatedPNGEncoder.h"

std::vector<uint8_t> readFromFile(const std::string &file)
{
    std::ifstream stream(file, std::ios::binary | std::ios::ate);

    if (!stream)
        throw std::runtime_error("Failed to open file: " + file);

    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!stream.read(reinterpret_cast<char *>(buffer.data()), size))
        throw std::runtime_error("Failed to read file: " + file);

    return buffer;
}

int main(int argc, char *argv[])
{
    const std::string input = argv[1];
    const std::string output = argv[2];

    // Read file to buffer
    std::cout << "Reading from file '" << input << "'..." << std::endl;
    std::vector<uint8_t> buffer = readFromFile(input);

    // Convert buffer
    std::cout << "Converting file..." << std::endl;
    std::unique_ptr<Converter> converter = ConverterFactory::create(buffer);
    const Image image = converter.get()->process();

    //  Output
    std::vector<Frame> frames = {
        {282, 204, 2, 790},
        {282, 204, 2, 790},
        {278, 194, 562, 1208},
        {278, 194, 562, 1208},
        {284, 188, 290, 596},
        {284, 188, 290, 596},
        {290, 194, 2, 386},
        {290, 194, 2, 386},
        {296, 196, 308, 2},
        {296, 196, 308, 2},
        {296, 194, 2, 190},
        {296, 194, 2, 190},
        {292, 200, 300, 200},
        {292, 200, 300, 200},
        {286, 204, 584, 588},
        {286, 204, 584, 588},
        {278, 208, 570, 998},
        {278, 208, 570, 998},
        {282, 206, 290, 786},
        {282, 206, 290, 786},
        {288, 200, 594, 386},
        {288, 200, 594, 386},
        {282, 190, 286, 994},
        {282, 190, 286, 994},
        {282, 186, 2, 996},
        {282, 186, 2, 996},
        {296, 192, 606, 2},
        {296, 192, 606, 2},
        {304, 186, 2, 2},
        {304, 186, 2, 2},
        {296, 188, 606, 196},
        {296, 188, 606, 196},
        {288, 192, 294, 402},
        {288, 192, 294, 402},
        {286, 206, 2, 582},
        {286, 206, 2, 582},
        {278, 210, 2, 1184},
        {278, 210, 2, 1184},
        {278, 206, 282, 1186},
        {278, 206, 282, 1186},
        {282, 202, 574, 794},
        {282, 202, 574, 794},
        {266, 182, 282, 1394},
        {266, 182, 282, 1394},
        {250, 184, 2, 1584, true},
        {250, 184, 2, 1584, true},
        {254, 186, 2, 1396},
        {254, 186, 2, 1396},
        {246, 186, 2, 1836},
        {246, 186, 2, 1836},
        {202, 170, 850, 998, true},
        {202, 170, 850, 998, true},
        {96, 132, 904, 204},
        {96, 132, 904, 204},
        {102, 102, 904, 2},
        {102, 102, 904, 2},
        {96, 94, 904, 338},
        {96, 94, 904, 338},
        {102, 96, 904, 106},
        {102, 96, 904, 106}};

    std::cout << "Encoding to PNG..." << std::endl;
    PNGEncoder(image).process("output-spritesheet.png");

    std::cout << "Encoding to APNG..." << std::endl;
    AnimatedPNGEncoder(image, frames, 326, 298).process("output-animated.png");

    return 0;
}