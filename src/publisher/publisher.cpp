#include "publisher/publisher.h"

using Declaration = Publisher::Declaration;

Publisher::Publisher(Fault& fault) 
: fault(fault), table() {}

Publisher::~Publisher() {
    for(std::pair<str, Declaration*> pair : table) delete pair.second;
}
