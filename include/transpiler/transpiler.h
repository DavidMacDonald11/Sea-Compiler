#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "output-file.h"

struct Transpiler {
    str lines;
    OutputFile& file;

    Transpiler(OutputFile& file);
    ~Transpiler();

    void write(str string = "", str end = "\n");
};

#endif //TRANSPILER_H
