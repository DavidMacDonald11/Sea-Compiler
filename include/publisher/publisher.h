#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <atomic>
#include "fault.h"
#include "publisher/publisher-file.h"
#include "util.h"

struct Publisher {
    class Value {};
    class Declaration;

    Fault& fault;
    PublisherFile& file;
    map<str, Declaration*> table;

    Publisher(Fault& fault, PublisherFile& file);
    ~Publisher();
};

struct Publisher::Declaration : public Value {
    
};

#endif //PUBLISHER_H
