#include "../../include/Readers/Reader.h"
#include "../../include/Readers/DDS/ATCAReader.h"
#include "../../include/Readers/DDS/ATCIReader.h"
#include "../../include/Readers/DDS/DDSReader.h"
#include <fstream>

Reader::Reader(const BinaryStream& r) : reader(r) {
}

std::vector<uint8_t> Reader::fromFile(const std::string_view file) {
    std::ifstream steam(file.data(), std::ios::binary | std::ios::ate);

    if (!steam)
        throw std::runtime_error("Failed to open file.");

    const std::streamsize size = steam.tellg();
    steam.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!steam.read(reinterpret_cast<char*>(buffer.data()), size))
        throw std::runtime_error("Failed to read file.");

    return buffer;
}
std::unique_ptr<Reader> Reader::create(std::span<const uint8_t> buffer) {
    BinaryStream r(buffer);

    std::string fourCC = r.readFixedString(4);

    if (fourCC == "DDS ") {
        const Header header = DDSReader::processHeader(r);

        fourCC = header.pixelFormat.fourCC;

        if (fourCC == "ATCA")
            return std::make_unique<ATCAReader>(r, header);

        if (fourCC == "ATCI")
            return std::make_unique<ATCIReader>(r, header);

        throw std::runtime_error("Unsupported DDS format: " + fourCC);
    }

    throw std::runtime_error("Unsupported format: " + fourCC);
}