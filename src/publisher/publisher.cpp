#include "publisher/publisher.h"

using Declarator = Publisher::Declarator;
using Type = Publisher::Type;
using Declaration = Publisher::Declaration;

Publisher::Publisher(Fault& fault) 
: fault(fault), table() {}

Publisher::~Publisher() {
    for(std::pair<str, Declaration*> pair : table) delete pair.second;
}


Declarator::~Declarator() {
    delete declarator;
}


Declaration::~Declaration() {
    delete type;
    delete declarator;
}
