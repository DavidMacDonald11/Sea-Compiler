#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <atomic>
#include "fault.h"
#include "util.h"

struct Publisher {
    class Value {};
    class Declaration;

    Fault& fault;
    map<str, Declaration*> table;

    Publisher(Fault& fault);
    ~Publisher();
};

struct Publisher::Declaration : public Value {
    
};

#endif //PUBLISHER_H
