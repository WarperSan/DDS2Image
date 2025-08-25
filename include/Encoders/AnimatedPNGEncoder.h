#include "Encoder.h"
#include <apngasm.h>
#include <string.h>
#include <stdexcept>

struct Frame
{
    uint32_t cropWidth;
    uint32_t cropHeight;
    uint32_t cropX;
    uint32_t cropY;
    bool rotated;
};

class AnimatedPNGEncoder : public Encoder
{
private:
    const std::vector<Frame> frames;
    const uint32_t targetWidth;
    const uint32_t targetHeight;

    void assignPixels(unsigned char *data, uint32_t frameWidth, uint32_t frameHeight, const Frame &frame)
    {
        memset(data, 0, frameWidth * frameHeight * 4);

        for (unsigned y = 0; y < frameHeight; y++)
        {
            for (unsigned x = 0; x < frameWidth; x++)
            {
                unsigned scaled_x = (x * frame.cropWidth) / frameWidth;
                unsigned scaled_y = (y * frame.cropHeight) / frameHeight;

                unsigned index = 0;

                if (frame.rotated)
                    index = (scaled_x + frame.cropY) * width + (scaled_y + frame.cropX);
                else
                    index = (scaled_y + frame.cropY) * width + (scaled_x + frame.cropX);

                const uint32_t color = pixels[index];

                unsigned char *p = &data[(y * frameWidth + x) * 4];
                p[0] = (color >> 24) & 0xFF; // R
                p[1] = (color >> 16) & 0xFF; // G
                p[2] = (color >> 8) & 0xFF;  // B
                p[3] = (color >> 0) & 0xFF;  // A
            }
        }
    }

protected:
    void encode(const std::string &filename) override
    {
        unsigned char *data = (unsigned char *)malloc(targetWidth * targetHeight * 4);

        if (!data)
            throw std::runtime_error("Could not allocate the necessary memory to encode the image.");

        apngasm::APNGAsm assembler;

        for (const auto &f : frames)
        {
            assignPixels(data, targetWidth, targetHeight, f);

            assembler.addFrame((apngasm::rgba *)data, targetWidth, targetHeight, 1, 30);
        }

        free(data);

        assembler.assemble(filename);
    }

public:
    AnimatedPNGEncoder(const Image &image, const std::vector<Frame> &frames, const uint32_t targetWidth, const uint32_t targetHeight) : Encoder(image), frames(frames), targetWidth(targetWidth), targetHeight(targetHeight) {}
};