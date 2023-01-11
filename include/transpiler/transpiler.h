#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "output-file.h"

class Node;

struct Transpiler {
    str lines;
    OutputFile& file;
    nat indent;

    Transpiler(OutputFile& file);
    ~Transpiler();

    void write(str string = "", str end = "\n");

    struct Line;
};

class Transpiler::Line {
    bool showType;
    bool finished;
    vector<Line> lines;
public:
    str type;
    str string;
    bool future;
    nat pointers;

    Line(str type = "", str string = "");

    str toString() const;
    Line& setShowType();
    Line& finish(const Node& node, bool semicolons = true);
    Line& replace(str string);
    Line& add(str before = "", str after = "");
    Line& cast(str type);
    Line& castUp();
    Line& prefix(const Line& line);

    static Line resolve(const Line& left, const Line& right);
};

#endif //TRANSPILER_H
