#include "parser/declarations/type-name.h"
#include "parser/declarations/specifier-qualifier-list.h"
#include "parser/declarations/abstract-declarator.h"
#include "publisher/publisher.h"
#include "transpiler/transpiler.h"

TypeName::TypeName(Node& list, Node* declarator)
: list(list), declarator(declarator) {}

TypeName::~TypeName() {
    delete &list;
    delete declarator;
}

Nodes TypeName::nodes() const {
    Nodes nodes = {&list};
    if(declarator) nodes.push_back(declarator);
    return nodes;
}

Node* TypeName::construct(Parser& parser) {
    Node* list = SpecifierQualifierList::construct(parser);
    return new TypeName(*list, AbstractDeclarator::construct(parser));
}

Publisher::Value* TypeName::publish(Publisher &publisher) {
    Publisher::Type* type = static_cast<Publisher::Type*>(list.publish(publisher));
    
    Publisher::Declarator* dec = nullptr;
    if(declarator) dec = static_cast<Publisher::Declarator*>(declarator->publish(publisher));

    return new Publisher::Declaration(type, dec);
}

Transpiler::Line TypeName::transpile(Transpiler& transpiler) {
    Transpiler::Line line = list.transpile(transpiler);

    if(declarator) {
        Transpiler::Line declarator = self.declarator->transpile(transpiler);
        line.add("", declarator.toString());
        line.pointers = declarator.pointers;
    }

    return line;
}
