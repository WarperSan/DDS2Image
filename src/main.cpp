#include "../include/Readers/Reader.h"
#include "Writers/Writer.h"
#include <memory>
#include <span>
#include <string_view>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string_view input  = argv[1];
    const std::string_view output = argv[2];

    // Read file to buffer
    std::vector<uint8_t> buffer = Reader::fromFile(input);

    // Convert buffer
    const std::unique_ptr<Reader> reader = Reader::create(buffer);

    const auto image = reader->process();

    // Output
    const std::unique_ptr<Writer> writer = Writer::create(output.substr(output.find_last_of('.')), image);

    const auto outputData = writer->process();

    Writer::toFile(output, outputData);

    return 0;
}