#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <map>
#include "fault.h"
#include "util.h"

using std::map;

class Declaration;

struct Publisher {
    static map<str, Declaration> table;
    Fault& fault;

    Publisher(Fault& fault);
};

struct Declaration {

};

#endif //PUBLISHER_H
