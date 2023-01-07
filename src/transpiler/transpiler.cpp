#include "parser/node.h"
#include "transpiler/transpiler.h"

Transpiler::Transpiler(OutputFile& file) 
: lines(), file(file), indent(0) {}

Transpiler::~Transpiler() {
    file.write(lines);
}

void Transpiler::write(str string, str end) {
    lines += string + end;
}

using Line = Transpiler::Line;

Line::Line(str type, str string)
: showType(false), finished(false), type(type), string(string) {}

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

    str indent;
    for(nat i = 0; i < node.transpiler->indent; i++) indent += "    ";

    str end = semicolons? ";" : "";
    end = showType? fmt::format("{} /*{}*/", end, type) : end;

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

Line& Line::prefix(const Line& line) {
    lines.push_back(line);
    return self;
}
