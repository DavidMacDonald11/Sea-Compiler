#include "publisher/publisher.h"

map<str, Declaration> Publisher::table;

Publisher::Publisher(Fault& fault) 
: fault(fault) {}
