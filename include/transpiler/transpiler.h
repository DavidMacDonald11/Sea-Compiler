#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "output-file.h"

class Node;

struct Transpiler {
    str lines;
    OutputFile& file;

    Transpiler(OutputFile& file);
    ~Transpiler();

    void write(str string = "", str end = "\n");

    struct Expression;
};

class Transpiler::Expression {
    bool showType;
    bool finished;
public:
    str type;
    str string;

    Expression(str type = "", str string = "");

    str toString() const;
    Expression& setShowType();
    Expression& finish(const Node& node, bool semicolons = true);
    Expression& replace(str string);
    Expression& add(str before = "", str after = "");
    Expression& cast(str type);
};

#endif //TRANSPILER_H
