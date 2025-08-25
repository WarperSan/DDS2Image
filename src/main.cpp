#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/BinaryReader.h"
#include "../include/Converter.h"
#include "../include/Factory.h"
#include "../include/stb_image_write.h"
#include <array>
#include <bit>
#include <fstream>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

std::vector<uint8_t> readFromFile(std::string_view file) {
  std::ifstream steam(file.data(), std::ios::binary | std::ios::ate);

  if (!steam)
    throw std::runtime_error("Failed to open file.");

  std::streamsize size = steam.tellg();
  steam.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(size);

  if (!steam.read(reinterpret_cast<char *>(buffer.data()), size))
    throw std::runtime_error("Failed to read file.");

  return buffer;
}

Result convertBuffer(std::span<const uint8_t> buffer) {
  std::unique_ptr<Converter> converter = Factory::create(buffer);

  return converter.get()->process();
}

void savePNG(std::string_view filename, std::span<uint32_t> rgbaPixels,
             int width, int height) {
  stbi_write_png(filename.data(), width, height, 4,
                 reinterpret_cast<uint8_t *>(rgbaPixels.data()), width * 4);
}

int main(int argc, char *argv[]) {
  std::string_view input = argv[1];
  std::string_view output = argv[2];

  // Read file to buffer
  std::vector<uint8_t> buffer = readFromFile(input);

  // Convert buffer
  Result result = convertBuffer(buffer);

  // Output
  savePNG(output, result.data, result.width, result.height);

  return 0;
}