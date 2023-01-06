#include "transpiler/transpiler.h"

Transpiler::Transpiler(OutputFile& file) 
: lines(), file(file) {}

Transpiler::~Transpiler() {
    file.write(lines);
}

void Transpiler::write(str string, str end) {
    lines += string + end;
}
