#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include "source_line.h"

class SourceFile {
private:
    std::ifstream file;
    vector<SourceLine*> lines;
public:
    const str& path;
    SourceLine* line;

    SourceFile(const str& filePath);
    ~SourceFile();

    void readLine();
    char next();
    str take(nat num = -1, const str& these = "", const str& until = "");
};

#endif
