#include "lexer/lexer.h"
#include "util/fault.h"

Lexer::Lexer(SourceFile& file)
: tokens(), file(file) {}

str Lexer::toString() const {
    str string;

    for(const Token& token : tokens) {
        str tString = token.toString();
        string = (string == "") ? tString : fmt::format("{}, {}", string, tString);
    }

    return string;
}

Token Lexer::newToken(Token::Type type) {
    Token token(*file.line, type);
    tokens.push_back(token);
    return token;
}

void Lexer::makeTokens() {
    do makeToken(); while(file.line->string != "");
}

void Lexer::makeToken() {
    ignoreSpaces();
    char next = file.next();

    if(next == '\0' or in(next, Token::PUNC_SYMS)) {
        file.take(1);
        Token token = newToken(Token::PUNC);
        return;
    }

    if(in(next, Token::OP_SYMS)) {
        makeOperator();
        return;
    }

    file.take(1);
    Token token = newToken(Token::NONE);
    throw Fault::fail(token, "Unrecognized symbol");
}

void Lexer::ignoreSpaces() {
    while(isspace(file.next())) { file.take(1); }
}

void Lexer::makeOperator() {
    str string;

    while(in(file.next(), Token::OP_SYMS)) {
        again:
        for(str op : Token::OPERATORS) {
            if(isSubstr(string + file.next(), op)) {
                string += file.take(1);
                goto again;
            }
        }

        newToken(Token::OP);
        return;
    }
}
