#include "lexer/lexer.h"
#include "lexer/source-file.h"
#include "lexer/source-line.h"
#include "util.h"

Lexer::Lexer(Fault& fault, SourceFile& file)
: fault(fault), tokens(), file(file) {}

str Lexer::toString() const { return vectorToString(tokens); }

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

    if(next == '\0' or in(next, Token::PUNC_SYMS)) return makePunctuator();
    if(next == '\\') return makeLineContinue();
    if(next == '\'') return makeChar();
    if(next == '"') return makeString();
    if(in(next, Token::S_NUM_SYMS)) return makeNumber();
    if(in(next, Token::OP_SYMS)) return makeOperator();
    if(in(next, Token::S_IDENTIFIER_SYMS)) return makeIdentifier();

    file.take(1);
    Token token = newToken(Token::NONE);

    throw fault.fail(token, fmt::format(
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

void Lexer::ignoreComment() {
    if(file.take(1) == "/") {
        file.take(-1, "", "\n");
        file.line->ignore();
        return;
    } 

    SourceLine& line = *file.line;

    while(file.next()) {
        if(file.next() != '*') {
            file.take(-1, "", "*");
            continue;
        }

        file.take(1);

        if(file.next() == '/') {
            file.take(1);
            file.line->ignore();
            return;
        }
    }

    Token token = newToken(Token::NONE, line);
    throw fault.fail(token, "Unterminated multiline comment");
}

void Lexer::makePunctuator() {
    file.take(1);
    newToken(Token::PUNC);
}

void Lexer::makeLineContinue() {
    file.take(1);

    while(isspace(file.next())) {
        if(file.take(1) == "\n") return file.line->ignore();
    }

    file.line->ignore();
    file.take(-1, "", "\n");

    Token token = newToken(Token::NONE);
    throw fault.fail(token, "Unexpected symbols after line continuation");
}

void Lexer::makeNewline() {
    SourceLine& line = *file.line;

    while(file.next() == '\n') {
        file.take(1);
    }
    
    newToken(Token::PUNC, line);
}

void Lexer::makeChar() {
    file.take(1);
    
    if(file.next() == '\\') { 
        file.take(1);

        if(file.next() == '\'') file.take(1);
        file.take(-1, "", "'");
    } else file.take(1);

    if(file.take(1) == "'") {
        newToken(Token::CHAR);
        return;
    }
        
    Token token = newToken(Token::NONE);
    throw fault.fail(token, "Unterminated character");
}

void Lexer::makeString() {
    file.take(1);

    while(file.next() != '\n') {
        str taken = file.take(1);
        if(taken == "\\") file.take(1);

        if(taken == "\"") {
            newToken(Token::STR);
            return;
        }
    }

    Token token = newToken(Token::NONE);
    throw fault.fail(token, "Unterminated string");
}

void Lexer::makeNumber() {
    if(file.take(1) == "." and not in(file.next(), str("0123456789"))) {
        Token::Type type = Token::OP;
        
        if(file.next() == '.') {
            type = Token::PUNC;
            file.take(1);
            if(file.next() == '.') file.take(1); 
        }
        
        newToken(type);
        return;
    }

    file.take(-1, Token::NUM_SYMS);
    newToken(Token::NUM);
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

    if(string == "/" and in(file.next(), str("/*"))) return ignoreComment();
    Token token = newToken(in(string, Token::PUNC_OPS)? Token::PUNC : Token::OP);
    if(string == "###") return makeCStatement();

    if(not in(string, Token::OPERATORS)) {
        throw fault.fail(token, fmt::format(
            "Unrecognized operator '{}'", 
            token.string));
    }
}

void Lexer::makeIdentifier() {
    str string = file.take(-1, Token::IDENTIFIER_SYMS);
    newToken(in(string, Token::KEYWORDS)? Token::KEYWORD : Token::IDENTIFIER);
}

void Lexer::makeCStatement() {
    file.take(-1, "", "\n");
    newToken(Token::C_TOKENS);
}
