#include "parser/declarations/function-definition.h"
#include "parser/declarations/function-specifiers.h"
#include "parser/declarations/parameters.h"
#include "parser/declarations/type-name.h"
#include "parser/statements/compound-statement.h"
#include "parser/statements/statement.h"

FunctionDefinition::FunctionDefinition(Node* specifiers, Token& name, Node* params, Node* type, Node& statement)
: specifiers(specifiers), name(name), parameters(params), type(type), statement(statement) {}

FunctionDefinition::~FunctionDefinition() {
    delete specifiers;
    delete parameters;
    delete type;
    delete &statement;
}

Nodes FunctionDefinition::nodes() const {
    Nodes nodes;

    if(specifiers) nodes.push_back(specifiers);
    nodes.push_back(&name);
    if(parameters) nodes.push_back(parameters);
    if(type) nodes.push_back(type);
    nodes.push_back(&statement);

    return nodes;
}

Node* FunctionDefinition::construct() {
    Token* visibility = (parser->next().has(Token::VISIBILITY_KEYWORDS))? &parser->take() : nullptr;
    Token* async = (parser->next().has({"async"}))? &parser->take() : nullptr;

    if(not async and not parser->next().has(mergeAll<str>({
            Token::FUNCTION_KEYWORDS, {"fun"}}))) {
        if(async) parser->i -= 1;
        if(visibility) parser->i -= 1;
        return nullptr;
    }

    if((async or parser->next().has({"fun"})) and not parser->ahead(1).of({
            Token::IDENTIFIER}) and not parser->next().has(Token::FUNCTION_KEYWORDS)) {
        if(async) parser->i -= 1;
        if(visibility) parser->i -= 1;
        
        return Statement::newLineStatement();
    }
    
    if(async) parser->i -= 1;
    if(visibility) parser->i -= 1;

    Node* specifiers = FunctionSpecifiers::construct();
    parser->expectingHas({"fun"});
    Token& name = parser->expectingOf({Token::IDENTIFIER});

    parser->expectingHas({"("});
    Node* parameters = nullptr;
    if(not parser->next().has({")"})) parameters = Parameters::construct();
    parser->expectingHas({")"});

    Node* type = nullptr;

    if(parser->next().has({"->"})) {
        parser->take();
        type = TypeName::construct();
    }

    Node* cStatement = CompoundStatement::construct();
    if(not cStatement) parser->expectingHas({"{"});
    
    Node& statement = *cStatement;
    return new FunctionDefinition(specifiers, name, parameters, type, statement);
}
