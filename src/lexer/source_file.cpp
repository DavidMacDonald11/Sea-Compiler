#include "lexer/source_file.h"
#include "lexer/source_line.h"

SourceFile::SourceFile(const str& filePath)
: file(std::ifstream(filePath)), path(filePath), line(nullptr) {
    readLine();
}

SourceFile::~SourceFile() {
    file.close();
}

void SourceFile::readLine() {
    nat num = (line == nullptr) ? 1 : line->num + 1;
    delete line;
    line = new SourceLine(num);

    char c;

    while(file.good()) {
        c = file.get();
        if(c == EOF) return;
        line->string += c;

        if(c == '\n') return;
    }
}

char SourceFile::next() {
    str string = line->unreadString();
    return (string != "") ? string[0] : '\0';
}

str SourceFile::take(nat num, const str& these, const str& until) {
    str string;

    if(line == nullptr or num == 0) return string;

    for(char c : line->unreadString()) {
        if(in(c, until) or (these != "" and in(c, these)))
            return string;

        string += c;
        line->increment();

        if(line->unreadString() == "" and line->string != "") readLine();
        if(num > 0 and string.length() == num) break;
    }

    return string;
}
