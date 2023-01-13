#include "parser/component.h"
#include "parser/declarations/initializer-list.h"
#include "parser/expressions/expression.h"
#include "parser/expressions/primary-expression.h"
#include "parser/node.h"
#include "fault.h"
#include "transpiler/transpiler.h"

Node* PrimaryExpression::construct() {
    Token& next = parser->next();

    if(next.of({Token::NUM})) return NumericConstant::construct();
    if(next.of({Token::CHAR})) return CharacterConstant::construct();
    if(next.of({Token::STR})) return StringConstant::construct();
    if(next.of({Token::IDENTIFIER})) return FileIdentifier::construct();
    if(next.has({"("})) return ParentheseseExpression::construct();
    if(next.has({"["})) return InitializerListExpression::construct();
    if(next.has(Token::PRIMARY_KEYWORDS)) return PrimaryKeyword::construct();

    Token& failure = parser->take();

    throw Fault::fail(failure, fmt::format(
        "PrimaryExpression error; unexpected token {}", 
        failure.toString()));
}


NumericConstant::NumericConstant(Token& token) 
: PrimaryNode(token) {}

Node* NumericConstant::construct() {
    return new NumericConstant(parser->take());
}

Transpiler::Line NumericConstant::transpile() {
    str type = in('.', token.string)? "float" : "int";
    return {type, token.string};
}


CharacterConstant::CharacterConstant(Token& token)
: PrimaryNode(token) {}

Node* CharacterConstant::construct() {
    return new CharacterConstant(parser->take());
}

Transpiler::Line CharacterConstant::transpile() {
    return {"char", token.string};
}


StringConstant::StringConstant(Token& token)
: PrimaryNode(token) {}

Node* StringConstant::construct() {
    return new StringConstant(parser->take());
}

Transpiler::Line StringConstant::transpile() {
    Transpiler::Line line = {"char", token.string};
    line.pointers = 1;
    return line;
}


vector<Component*> FileIdentifier::nodes() const {
    return {&file, &identifier};
}

FileIdentifier::FileIdentifier(Token& file, Token& identifier) 
: file(file), identifier(identifier) {}

Node* FileIdentifier::construct() {
    if(not parser->ahead(1).has({"::"})) return Identifier::construct();
    
    Token& file = parser->expectingOf({Token::IDENTIFIER});
    parser->take();
    Token& identifier = parser->expectingOf({Token::IDENTIFIER});

    return new FileIdentifier(file, identifier);
}

Transpiler::Line FileIdentifier::transpile() {
    Transpiler::Line line = {"?"};

    if(file.string != "C" and file.string != "c") line.add(file.string + "_");
    line.add("", identifier.string);

    return line;
}


Identifier::Identifier(Token& token)
: PrimaryNode(token) {}

Node* Identifier::construct() {
    return new Identifier(parser->expectingOf({Token::IDENTIFIER}));
}

Transpiler::Line Identifier::transpile() {
    str type = "IDENTIFIER";
    return {type, token.string};
}


vector<Component*> ParentheseseExpression::nodes() const { return {&expression}; }

ParentheseseExpression::ParentheseseExpression(Node& expression)
: expression(expression) {}

ParentheseseExpression::~ParentheseseExpression() {
    delete &expression;
}

Node* ParentheseseExpression::construct() {
    parser->expectingHas({"("});
    parser->skipNewlines();

    Node* node = Expression::construct();

    parser->skipNewlines();
    parser->expectingHas({")"});

    return new ParentheseseExpression(*node);
}

Transpiler::Line ParentheseseExpression::transpile() {
    return expression.transpile().add("(", ")");
}


InitializerListExpression::InitializerListExpression(Node* initializerList)
: initializerList(initializerList) {}

InitializerListExpression::~InitializerListExpression() {
    delete initializerList;
}

Nodes InitializerListExpression::nodes() const {
    return initializerList? Nodes{initializerList} : Nodes{};
}

Node* InitializerListExpression::construct() {
    parser->expectingHas({"["});
    parser->skipNewlines();

    if(parser->next().has({"]"})) {
        parser->take();
        return new InitializerListExpression(nullptr);
    }

    Node* node = InitializerList::construct();
    if(parser->next().has({","})) parser->take();

    parser->skipNewlines();
    parser->expectingHas({"]"});
    
    return new InitializerListExpression(node);
}

Transpiler::Line InitializerListExpression::transpile() {
    if(not initializerList) return {"", "{}"};
    return initializerList->transpile().add("{", "}");
}


PrimaryKeyword::PrimaryKeyword(Token& token)
: PrimaryNode(token) {}

Node* PrimaryKeyword::construct() {
    return new PrimaryKeyword(parser->take());
}

Transpiler::Line PrimaryKeyword::transpile() {
    str type = (token.string == "null")? "byte" : "bool";
    str string = (token.string == "true")? "1" : "0";
    return {type, string};
}
