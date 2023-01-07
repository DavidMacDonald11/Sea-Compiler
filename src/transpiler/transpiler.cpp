#include <map>
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "util.h"

Transpiler::Transpiler(OutputFile& file) 
: lines(), file(file), indent(0) {}

Transpiler::~Transpiler() {
    file.write(lines);
}

void Transpiler::write(str string, str end) {
    lines += string + end;
}


using Line = Transpiler::Line;

std::map<str, double> points {
    {"wild", 100},
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

Line& Line::finish(const Node& node, bool semicolons) {
    for(Line& line : lines) line.finish(node, semicolons);

    if(finished) return self;
    finished = true;

    str indent = multiplyStr("    ", node.transpiler->indent);
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

Line& Line::cast(str type) {
    self.type = type;
    return self;
}

Line& Line::castUp() {
    type = in(type, vector<str>{"bool", "byte", "char"})? "short nat" : type;
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
