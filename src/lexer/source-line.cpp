#include "lexer/token.h"
#include "lexer/source-line.h"

using Locale = SourceLine::Locale;

SourceLine::SourceLine(nat num)
: locale({0, 0}), num(num), string(), marks() {}

void SourceLine::ignore() {
    locale[0] = locale[1];
}

void SourceLine::mark(Token& token) { mark(token.locale); }
void SourceLine::mark(Locale locale) { marks.push_back(locale); }

void SourceLine::increment() { locale[1] += 1; }

Locale SourceLine::newLocale() {
    Locale result(locale);
    locale[0] = locale[1];

    return result;
}

str SourceLine::unreadString() const {
    return string.substr(locale[1]);
}

str SourceLine::raw() const {
    nat col1 = 0, col2 = 0;

    for(const Locale& locale : marks) {
        if(col1 == 0 or locale[0] < col1) col1 = locale[0];
        if(col2 == 0 or locale[1] > col2) col2 = locale[1];
    }

    str underline;

    for(nat i = 0; i < string.size(); i++) {
        underline += (i < col1 or i >= col2) ? ' ' : '^';
    }

    str line = (string == "")? "EOF\n" : string;
    if(string == "") underline = "^^^";
    
    return fmt::format("{:4d}|{}    |{}", num, line, underline);
}
