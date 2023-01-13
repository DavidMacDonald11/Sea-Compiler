#include "parser/declarations/function-type-specifier.h"
#include "parser/declarations/abstract-parameters.h"
#include "parser/declarations/type-name.h"

FunctionTypeSpecifier::FunctionTypeSpecifier(Token* async, Token& fun ,Node* parameters, Node* type)
: async(async), fun(fun), parameters(parameters), type(type) {}

FunctionTypeSpecifier::~FunctionTypeSpecifier() {
    delete parameters;
    delete type;
}

Nodes FunctionTypeSpecifier::nodes() const {
    Nodes nodes;

    if(async) nodes.push_back(async);
    nodes.push_back(&fun);
    if(parameters) nodes.push_back(parameters);
    if(type) nodes.push_back(type);

    return nodes;
}

Node* FunctionTypeSpecifier::construct() {
    Token* async = (parser->next().has({"async"}))? &parser->take() : nullptr;
    Token& fun = parser->expectingHas({"fun"});

    parser->expectingHas({"("});
    Token* paren = (parser->next().has({"("}))? &parser->take() : nullptr;

    Node* parameters = nullptr;
    bool allow = parser->context.allowDirectAbstractDeclarator;
    
    parser->context.allowDirectAbstractDeclarator = false;
    if(not parser->next().has({")"})) parameters = AbstractParameters::construct();
    parser->context.allowDirectAbstractDeclarator = allow;

    parser->expectingHas({")"});
    Node* type = nullptr;

    if(paren and parser->next().has({"->"})) {
        parser->take();

        Token* paren = (parser->next().has({"("}))? &parser->take() : nullptr;
        type = TypeName::construct();
        if(paren) parser->expectingHas({")"});
    }

    if(paren) parser->expectingHas({")"});
    return new FunctionTypeSpecifier(async, fun, parameters, type);
}
