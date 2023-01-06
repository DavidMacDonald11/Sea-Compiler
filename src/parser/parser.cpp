#include "parser/node.h"
#include "parser/parser.h"
#include "parser/statements/file-statement.h"
#include "util/fault.h"

Parser::Parser(vector<Token>& tokens) 
: tokens(tokens), i(0), tree(nullptr) {}

Parser::~Parser() {
    delete tree;
}

void Parser::makeTree() {
    FileStatement* node = new FileStatement();
    node->makeTree();
    tree = node;
}

Token& Parser::next() const {
    return tokens[i > tokens.size() - 1 ? tokens.size() - 1: i];
}

Token& Parser::take() {
    Token& token = next();
    i += 1;
    return token;
}

Token& Parser::expectingOf(vector<Token::Type> types) {
    if(next().of(types)) return take();

    str message = fmt::format("Expecting of [{}]", join(types, ", "));
    throw Fault::fail(take(), message);
}

Token& Parser::expectingHas(vector<str> values) {
    if(next().has(values)) return take();

    str message = fmt::format("Expecting has [{}]", join(values, ", "));
    throw Fault::fail(take(), message);
}
