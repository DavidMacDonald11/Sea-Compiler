#include "publisher/publisher.h"
using Declaration = Publisher::Declaration;

Publisher::Publisher(Fault& fault, PublisherFile& file) 
: fault(fault), file(file), table() {}

Publisher::~Publisher() {
    for(std::pair<str, Value*> pair : table) delete pair.second;
}

str Publisher::toString() const {
    str result = "";

    for(std::pair<str, Value*> pair : table) {
        result += fmt::format("\t{}: {}\n", pair.first, pair.second->toString());
    }

    return result;
}


Declaration::Declaration(str name)
: name(name) {}

str Declaration::toString() const {
    return name;
}
