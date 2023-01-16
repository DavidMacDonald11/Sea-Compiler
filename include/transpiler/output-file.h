#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include "util.h"

class OutputFile {
    std::ofstream* file;
public:
    str path;

    OutputFile(str outDir, str srcPath);
    ~OutputFile();

    void write(const str& s);
};

#endif //OUTPUT_FILE_H
