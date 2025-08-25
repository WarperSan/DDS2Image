#pragma once

#include "BinaryReader.h"
#include <array>
#include <bit>
#include <cstdint>
#include <fstream>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

#define PACK_RGBA(r, g, b, a)                                                  \
  ((static_cast<uint32_t>(std::round(r)) << 24) |                              \
   (static_cast<uint32_t>(std::round(g)) << 16) |                              \
   (static_cast<uint32_t>(std::round(b)) << 8) |                               \
   (static_cast<uint32_t>(std::round(a))))

struct PixelFormat {
  uint32_t flags;
  std::string fourCC;
  uint32_t bpp;
  uint32_t redMask;
  uint32_t greenMask;
  uint32_t blueMask;
  uint32_t alphaMask;
};

struct Header {
  uint32_t flags;
  uint32_t height;
  uint32_t width;
  uint32_t pitcOrLinsize;
  uint32_t depth;
  uint32_t mipMapCount;
  PixelFormat pixelFormat;
  int32_t surfaceComplexity;
  int32_t additionalSurfaceDetails;
};

struct Result {
  uint32_t width;
  uint32_t height;
  std::vector<uint32_t> data;
};

class Converter {
protected:
  BinaryReader reader;
  const Header header;

  /**
   * @brief Processes the data representing the pixels
   *
   * @param data Data representing the pixels
   */
  virtual void processPixels(std::span<uint32_t> data) = 0;

  /**
   * @brief Processes the data representing the pixels
   *
   * @param pixel the pixel value
   * @return   the byte output in the order that stbi_write_png expects
   */
  inline static uint32_t setPixelRGBA(uint32_t pixel) {
    uint8_t r = (pixel >> 24) & 0xFF;
    uint8_t g = (pixel >> 16) & 0xFF;
    uint8_t b = (pixel >> 8) & 0xFF;
    uint8_t a = pixel & 0xFF;
    return std::bit_cast<uint32_t>(std::array{r, g, b, a});
  }

public:
  /**
   * @brief Construct a new Converter object
   *
   * @param r Reader to read the data from
   * @param h Metadata loaded from the data
   */
  Converter(BinaryReader &r, const Header &h) : reader(r), header(h) {}

  /**
   * @brief Porcesses the loaded data
   *
   * @return Result Raw information for an image
   */
  Result process();
};