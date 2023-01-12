#ifndef SOURCE_LINE_H
#define SOURCE_LINE_H

#include "../util.h"

class Token;

class SourceLine {
public: using Locale = array<nat, 2>;
private: Locale locale;
public:
    nat num;
    str string;
    vector<Locale> marks;

    SourceLine(nat num);

    void ignore();
    void mark(Token& token);
    void mark(Locale locale);
    void increment();
    Locale newLocale();
    str unreadString() const;
    str raw() const;
};

#endif
