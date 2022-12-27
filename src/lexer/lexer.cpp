#include "lexer/lexer.h"
#include "lexer/source_file.h"
#include "lexer/source_line.h"
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

Token Lexer::newToken(Token::Type type, SourceLine& line) {
    Token token(line, type);
    tokens.push_back(token);
    return token;
}

Token Lexer::newToken(Token::Type type) {
    return newToken(type, *file.line);
}

void Lexer::makeTokens() {
    do makeToken(); while(file.line->string != "");
}

void Lexer::makeToken() {
    ignoreSpaces();
    char next = file.next();

    if(next == '\0' or in(next, Token::PUNC_SYMS)) {
        file.take(1);
        newToken(Token::PUNC);
        return;
    }

    if(in(next, Token::OP_SYMS)) return makeOperator();
    if(in(next, Token::S_NUM_SYMS)) return makeNumber();

    file.take(1);
    Token token = newToken(Token::NONE);
    throw Fault::fail(token, "Unrecognized symbol");
}

void Lexer::ignoreSpaces() {
    while(isspace(file.next())) {
        if(file.next() == '\n') { 
            makeNewline();
            continue;
        }

        file.take(1);
        file.line->ignore();
    }
}    

void Lexer::makeNewline() {
    SourceLine& line = *file.line;

    while(file.next() == '\n') {
        file.take(1);
    }
    
    newToken(Token::PUNC, line);
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

void Lexer::makeNumber() {
    if(file.take(1) == "." and not in(file.next(), str("0123456789"))) {
        newToken(Token::OP);
        return;
    }

    file.take(-1, Token::NUM_SYMS);
    newToken(Token::NUM);
}
