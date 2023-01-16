#ifndef DEBUG_FILE_H
#define DEBUG_FILE_H

#include "util.h"

class DebugFile {
    std::ofstream* file;
public:
    str outDir;
    str srcPath;
    str path;

    DebugFile(str outDir, str srcPath, str options);
    ~DebugFile();

    void write(const str& s, str end = "\n\n");
};

#endif //DEBUG_FILE_H
