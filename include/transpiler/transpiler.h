#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "output-file.h"

class Node;

class Transpiler {
    vector<str> headers;
public:
    struct Context {
        nat indent;
    };

    str lines;
    OutputFile& file;
    Context context;

    Transpiler(OutputFile& file);
    ~Transpiler();

    void header(str string = "", str end = "\n");
    void write(str string = "", str end = "\n");
    void include(str header);

    class Line;
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
    Line& cast(str type, nat pointers);
    Line& cast(str type);
    Line& castUp();
    Line& prefix(const Line& line);

    static Line resolve(const Line& left, const Line& right);
};

#endif //TRANSPILER_H
