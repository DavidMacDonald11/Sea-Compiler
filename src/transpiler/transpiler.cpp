#include <map>
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "util.h"

Transpiler::Transpiler(Fault& fault, OutputFile& file) 
: headers(), fault(fault), lines(), file(file), context() {}

Transpiler::~Transpiler() {
    file.write(lines);
}

void Transpiler::header(str string, str end) {
    file.write(string + end);
}

void Transpiler::write(str string, str end) {
    lines += string + end;
}

void Transpiler::include(str header) {
    if(in(header, headers)) return;
    headers.push_back(header);
    self.header(fmt::format("#include <{}.h>", header));
}


using Line = Transpiler::Line;

std::map<str, double> points {
    {"?", 101}, {"wild", 100},
    {"bool", 0}, {"byte", .5}, {"char", 1},
    {"nat16", 2}, {"int16", 2.5}, 
    {"nat", 3}, {"int", 3.5}, 
    {"nat32", 4}, {"int32", 4.5}, 
    {"nat64", 5}, {"int64", 5.5},
    {"float32", 6}, {"float", 7}, {"float64", 8}
};

Line::Line(str type, str string)
: showType(false), finished(false), type(type), string(string), pointers(0) {}

str Line::toString() const { 
    str lines;
    for(const Line& line : self.lines) lines += line.toString();
    return lines + string;
}

Line& Line::setShowType() {
    showType = true;
    return self;
}

Line& Line::finish(const Node& node, const Transpiler& transpiler, bool semicolons) {
    for(Line& line : lines) line.finish(node, transpiler, semicolons);

    if(finished) return self;
    finished = true;

    str indent = multiplyStr("    ", transpiler.context.indent);
    str end = semicolons? ";" : "";
    str pointers = multiplyStr("^", self.pointers);

    end = showType? fmt::format("{} /*{}{}*/", end, type, pointers) : end;
    return add(indent, end + "\n");
}

Line& Line::replace(str string) { 
    self.string = string; 
    return self;
}

Line& Line::add(str before, str after) {
    string = before + string + after;
    return self;
}

Line& Line::cast(str type, nat pointers) {
    self.type = type;
    self.pointers = pointers;
    return self;
}

Line& Line::cast(str type) { return cast(type, pointers); }

Line& Line::castUp() {
    type = in(type, vector<str>{"bool", "byte", "char"})? "nat16" : type;
    return self;
}

Line& Line::prefix(const Line& line) {
    lines.push_back(line);
    return self;
}

Line Line::resolve(const Line& left, const Line& right) {
    str lType = left.type;
    str rType = right.type;

    return {(points[lType] > points[rType])? lType : rType};
} 
