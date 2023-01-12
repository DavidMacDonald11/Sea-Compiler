#include "parser/node.h"
#include "parser/parser.h"
#include "parser/statements/file-statement.h"
#include "fault.h"

Parser::Parser(vector<Token>& tokens) 
: tokens(tokens), i(0), tree(nullptr), context() {}

Parser::~Parser() {
    delete tree;
}

str Parser::toString() const {
    return tree? tree->toString() : "";
}

void Parser::makeTree() {
    FileStatement* node = new FileStatement();
    node->makeTree();
    tree = node;
}

Token& Parser::next() const {
    return tokens[(i > tokens.size() - 1)? tokens.size() - 1: i];
}

Token& Parser::ahead(nat pos) const {
    nat i = self.i + pos;
    return tokens[(i > tokens.size() - 1)? tokens.size() - 1: i];
}

Token& Parser::take() {
    Token& token = next();
    i += 1;
    return token;
}

bool Parser::nextOrAfterHas(vector<str> values) {
    return next().has(values) or ahead(1).has(values);
}

Token& Parser::expectingOf(vector<Token::Type> types) {
    if(next().of(types)) return take();

    vector<str> names;
    for(Token::Type type : types) names.push_back(Token::typeToString(type));

    str message = fmt::format("Expecting one of [{}]", join(names, ", "));
    throw Fault::fail(take(), message);
}

Token& Parser::expectingHas(vector<str> values) {
    if(next().has(values)) return take();

    vector<str> escaped;
    for(str value : values) escaped.push_back(fmt::format("\"{}\"", value));

    str message = fmt::format("Expecting one of [{}]", join(escaped, ", "));
    throw Fault::fail(take(), message);
}

void Parser::skipNewlines() {
    while(next().has({"\n"})) take();
}
