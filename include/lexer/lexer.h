#ifndef LEXER_H
#define LEXER_H

#include "fault.h"
#include "lexer/source-line.h"
#include "token.h"
#include "source-file.h"
#include "../util.h"

struct Lexer {
    Fault& fault;
    vector<Token> tokens;
    SourceFile& file;

    Lexer(Fault& fault, SourceFile& file);

    str toString() const;
    Token newToken(Token::Type type, SourceLine& line);
    Token newToken(Token::Type type);

    void makeTokens();
    void makeToken();
    void ignoreSpaces();
    void ignoreComment();
    void makePunctuator();
    void makeLineContinue();
    void makeNewline();
    void makeChar();
    void makeString();
    void makeNumber();
    void makeOperator();
    void makeIdentifier();
    void makeCStatement();
};

#endif
