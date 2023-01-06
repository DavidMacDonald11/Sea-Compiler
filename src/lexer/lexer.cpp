#include "lexer/lexer.h"
#include "lexer/source-file.h"
#include "lexer/source-line.h"
#include "fault.h"

Lexer::Lexer(SourceFile& file)
: tokens(), file(file) {}

str Lexer::toString() const {
    str string;

    for(const Token& token : tokens) {
        str tString = token.toString();
        string = (string == "")? tString : fmt::format("{}, {}", string, tString);
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

    if(in(next, Token::S_NUM_SYMS)) return makeNumber();
    if(in(next, Token::OP_SYMS)) return makeOperator();
    if(in(next, Token::S_IDENTIFIER_SYMS)) return makeIdentifier();

    file.take(1);
    Token token = newToken(Token::NONE);

    throw Fault::fail(token, fmt::format(
        "Unrecognized symbol '{}'",
        token.string
    ));
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
    str next;

    checkNextSym:
    next = string + file.next();

    for(str op : Token::OPERATORS) {
        if(isSubstr(next, op)) {
            string += file.take(1);
            goto checkNextSym;
        }
    }

    Token token = newToken(Token::OP);

    if(not in(string, Token::OPERATORS)) {
        throw Fault::fail(token, fmt::format(
            "Unrecognized operator '{}'", 
            token.string));
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

void Lexer::makeIdentifier() {
    str string = file.take(-1, Token::IDENTIFIER_SYMS);
    newToken(in(string, Token::KEYWORDS)? Token::KEYWORD : Token::IDENTIFIER);
}
