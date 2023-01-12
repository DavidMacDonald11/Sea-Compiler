#include "substitutor/substitutor.h"
#include "util.h"
#include "fault.h"
#include <cmath>
#include <string>

Substitutor::Substitutor(vector<Token>& tokens)
: lexerTokens(tokens), tokens(), i(0) {}

str Substitutor::toString() const { return vectorToString(tokens); }
Token& Substitutor::next() const { return lexerTokens[i]; }

Token& Substitutor::ahead(nat pos) const {
    nat i = self.i + pos;
    return lexerTokens[(i > lexerTokens.size() - 1)? lexerTokens.size() - 1 : i];
}

Token& Substitutor::take() {
    Token& token = next();
    i += 1;
    return token;
}

void Substitutor::skip() { i += 1; }
void Substitutor::add(Token token) { tokens.push_back(token); }

void Substitutor::replaceTokens() {
    while(i < lexerTokens.size()) checkToken();
}

void Substitutor::checkToken() {
    Token& token = next();

    if((token.has({"+", "-"}) 
        and ahead(1).of({Token::NUM}) 
        and ahead(2).has({"to"}))
        or (token.of({Token::NUM, Token::CHAR})
        and ahead(1).has({"to"})))
            return replaceRangeExpression();

    add(take());
}

void Substitutor::replaceRangeExpression() {
    Token* leftSign = (next().has({"+", "-"}))? &take() : nullptr;
    Token& left = take();
    skip();

    Token* rightSign = (next().has({"+", "-"}))? &take() : nullptr;

    if(not next().of({left.type})) {
        throw Fault::fail(next(), fmt::format(
            "Expecting {}", 
            Token::typeToString(left.type)));
    }

    Token& right = take();
    
    // TODO allow char and "by STEP"
    // TODO refactor

    if(not left.isInt()) 
        throw Fault::fail(left, fmt::format("Expecting integer"));
    
    if(not right.isInt())
        throw Fault::fail(right, fmt::format("Expecting integer"));

    str leftString = leftSign? leftSign->string : "";
    leftString += left.string;

    str rightString = rightSign? rightSign->string : "";
    rightString += right.string;

    long long leftNum = std::stoll(leftString);
    long long rightNum = std::stoll(rightString);

    SourceLine& line = left.line;

    Token::Locale locale = {
        (leftSign? leftSign : &left)->locale[0],
        right.locale[1]
    };

    while(leftNum <= rightNum) {
        if(leftNum < 0) add({line, Token::OP, "-", locale});

        str string = fmt::format("{}", std::abs(leftNum));
        add({line, Token::NUM, string, locale});
        leftNum += 1;

        if(leftNum <= rightNum) add({line, Token::PUNC, ",", locale});
        else return;
    }

    while(leftNum >= rightNum) {
        if(leftNum < 0) add({line, Token::OP, "-", locale});

        str string = fmt::format("{}", std::abs(leftNum));
        add({line, Token::NUM, string, locale});
        leftNum -= 1;

        if(leftNum >= rightNum) add({line, Token::PUNC, ",", locale});
    }
}
