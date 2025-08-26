#include "Readers/Reader.h"
#include "Writers/Writer.h"
#include <memory>
#include <span>
#include <string_view>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string_view input  = argv[1];
    const std::string_view output = argv[2];

    const std::string_view inputExtension  = input.substr(input.find_last_of('.'));
    const std::string_view outputExtension = output.substr(output.find_last_of('.'));

    if (inputExtension == outputExtension)
        throw std::invalid_argument("The input and the output cannot have the same extension.");

    // Read file to buffer
    std::vector<uint8_t> buffer = Reader::fromFile(input);

    // Convert buffer
    const std::unique_ptr<Reader> reader = Reader::create(buffer);

    const auto image = reader->process();

    // Output
    const std::unique_ptr<Writer> writer = Writer::create(outputExtension, image);

    const auto outputData = writer->process();

    Writer::toFile(output, outputData);

    return 0;
}