#include "./util/output_file.h"
#include <fstream>

OutputFile::OutputFile(const str& filePath)
: file(std::ofstream(filePath)), path(filePath) {}

OutputFile::~OutputFile() {
    file.close();
}

void OutputFile::write(const str& text) {
    file << text;
}