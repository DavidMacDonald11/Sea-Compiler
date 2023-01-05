#ifndef COMPONENT_H
#define COMPONENT_H

#include "lexer/source_line.h"
#include "util.h"

class Component {
public:
    virtual str toString() const = 0;
    virtual str tree(str prefix) const = 0;
    virtual vector<SourceLine*> lines() const = 0;
    virtual str raw() const = 0;
    virtual void mark() = 0;
    virtual ~Component() {};
};

#endif //COMPONENT_H
