#include "parser/node.h"
#include "transpiler/transpiler.h"

Transpiler::Transpiler(OutputFile& file) 
: lines(), file(file) {}

Transpiler::~Transpiler() {
    file.write(lines);
}

void Transpiler::write(str string, str end) {
    lines += string + end;
}

using Expression = Transpiler::Expression;

Expression::Expression(str type, str string)
: showType(false), finished(false), type(type), string(string) {}

str Expression::toString() const { return string; }

Expression& Expression::setShowType() {
    showType = true;
    return self;
}

Expression& Expression::finish(const Node&, bool semicolons) {
    if(finished) return self;
    finished = true;

    str indent = ""; // set indent w/ node.transpiler
    str end = semicolons? ";" : "";
    end = showType? fmt::format("{} /*{}*/", end, type) : end;

    return add(indent, end);
}

Expression& Expression::replace(str string) { 
    self.string = string; 
    return self;
}

Expression& Expression::add(str before, str after) {
    string = before + string + after;
    return self;
}

Expression& Expression::cast(str type) {
    self.type = type;
    return self;
}
