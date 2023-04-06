#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <atomic>
#include "fault.h"
#include "publisher/publisher-file.h"
#include "util.h"

struct Publisher {
    class Value;
    class Declaration;

    Fault& fault;
    PublisherFile& file;
    map<str, Value*> table;

    Publisher(Fault& fault, PublisherFile& file);
    ~Publisher();

    str toString() const;
};

struct Publisher::Value {
    virtual ~Value() {}
    virtual str toString() const = 0;
};

struct Publisher::Declaration : public Value {
    str name;

    Declaration(str name);

    str toString() const override;
};

#endif //PUBLISHER_H
