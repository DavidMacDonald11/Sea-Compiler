#include "publisher/publisher.h"
using Declaration = Publisher::Declaration;

Publisher::Publisher(Fault& fault, PublisherFile& file) 
: fault(fault), file(file), table() {}

Publisher::~Publisher() {
    for(std::pair<str, Declaration*> pair : table) delete pair.second;
}
