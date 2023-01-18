#ifndef PUBLISHER_FILE_H
#define PUBLISHER_FILE_H

#include "util.h"

class PublisherFile {
    std::ofstream* file;
public:
    str outDir;
    str srcPath;
    str path;

    PublisherFile(str outDir, str srcPath);
    ~PublisherFile();

    void write(const str& s, str end = "\n\n");
};

#endif //PUBLISHER_FILE_H
