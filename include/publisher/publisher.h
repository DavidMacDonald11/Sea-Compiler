#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <atomic>
#include "fault.h"
#include "util.h"

struct Publisher {
    class Value {};
    class BasicValue;
    class Declarator;
    class Declarators;
    class Type;
    class Declaration;

    Fault& fault;
    map<str, Declaration*> table;

    Publisher(Fault& fault);
    ~Publisher();
};

struct Publisher::BasicValue : public Value {
    str data;

    BasicValue(str data = "");
};

struct Publisher::Declarator : public Value {
    vector<str> qualifiers;
    Declarator* declarator;

    Declarator(vector<str> qualifiers = {}, Declarator* declarator = nullptr);
    ~Declarator();
};

struct Publisher::Declarators : public Value {
    vector<Declarator*> declarators;

    Declarators(vector<Declarator*> declarators = {});
    ~Declarators();
};

// TODO atomic + func types
struct Publisher::Type : public Value {
    vector<str> qualifiers;
    str file;
    str keyword;

    Type();
    Type(str keyword, str file = "");
    Type(vector<str> qualifiers);
};

struct Publisher::Declaration : public Value {
    str visibility;
    str storage;
    Type* type; 
    Declarator* declarator;

    Declaration(Type* type, Declarator* declarator);
    ~Declaration();
};

#endif //PUBLISHER_H
