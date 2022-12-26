#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include "util.h"

class OutputFile {
private:
    std::ofstream file;
public:
    const str& path;

    OutputFile(const str& filePath);
    ~OutputFile();

    void write(const str& text);
};

#endif //OUTPUT_FILE_H
