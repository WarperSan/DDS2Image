#ifndef DDS2IMAGE_FACTORY_H
#define DDS2IMAGE_FACTORY_H

#include "Reader.h"
#include <memory>

class Factory {
    public:
    /**
     * Creates the appropriate Reader from the given binary
     */
    static std::unique_ptr<Reader> create(std::span<const uint8_t> buffer);
};

#endif // DDS2IMAGE_FACTORY_H
