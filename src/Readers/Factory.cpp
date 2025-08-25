#include "../../include/Readers/Factory.h"
#include "../../include/Readers/DDS/ATCAReader.h"
#include "../../include/Readers/DDS/ATCIReader.h"
#include "../../include/Readers/DDS/DDSReader.h"

std::unique_ptr<Reader> Factory::create(const std::span<const uint8_t> buffer) {
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
