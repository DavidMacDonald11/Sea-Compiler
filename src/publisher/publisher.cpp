#include "publisher/publisher.h"

using BasicValue = Publisher::BasicValue;
using Declarator = Publisher::Declarator;
using Declarators = Publisher::Declarators;
using Type = Publisher::Type;
using Declaration = Publisher::Declaration;

Publisher::Publisher(Fault& fault) 
: fault(fault), table() {}

Publisher::~Publisher() {
    for(std::pair<str, Declaration*> pair : table) delete pair.second;
}


BasicValue::BasicValue(str data)
: data(data) {}


Declarator::Declarator(vector<str> qualifiers, Declarator* declarator) 
: qualifiers(qualifiers), declarator(declarator) {}

Declarator::~Declarator() {
    delete declarator;
}


Declarators::Declarators(vector<Declarator*> declarators)
: declarators(declarators) {}

Declarators::~Declarators() {
    for(Declarator* ptr : declarators) delete ptr;
}


Type::Type()
: qualifiers(), file(), keyword() {}

Type::Type(str keyword, str file)
: qualifiers(), file(file), keyword(keyword) {}

Type::Type(vector<str> qualifiers)
: qualifiers(qualifiers), file(), keyword() {}


Declaration::Declaration(Type* type, Declarator* declarator)
: visibility(), storage(), type(type), declarator(declarator) {}

Declaration::~Declaration() {
    delete type;
    delete declarator;
}
