#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <atomic>
#include <map>
#include "fault.h"
#include "util.h"

using std::map;

struct Publisher {
    class Value {};
    class Declarator;
    class Type;
    class Declaration;

    Fault& fault;
    map<str, Declaration*> table;

    Publisher(Fault& fault);
    ~Publisher();
};

struct Publisher::Declarator : public Value {
    vector<str> qualifiers;
    Declarator* declarator;

    ~Declarator();
};

struct Publisher::Type : public Value {
    vector<str> qualifiers;
    str keyword;
};

struct Publisher::Declaration : public Value {
    str visibility;
    str storage;
    Type* type; 
    Declarator* declarator;

    ~Declaration();
};

#endif //PUBLISHER_H
