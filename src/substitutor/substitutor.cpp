#include <cmath>
#include <map>
#include <string>
#include "substitutor/substitutor.h"
#include "util.h"
#include "fault.h"

static std::map<str, char> charMap {
    {"'\\?'", '\?'},
    {"'\\a'", '\a'},
    {"'\\b'", '\b'},
    {"'\\f'", '\f'},
    {"'\\n'", '\n'},
    {"'\\r'", '\r'},
    {"'\\t'", '\t'},
    {"'\\v'", '\v'},
    {"'\\''", '\''},
    {"'\\\"'", '\"'},
    {"'\\\\'", '\\'}
};

static std::map<char, str> strMap(invertMap(charMap));


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

Token Substitutor::combineSign(Token* sign, Token token) {
    if(sign and sign->has({"-"})) {
        token.string = "-" + token.string;
        token.locale[0] -= 1;
    }

    return token;
}

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
            return replaceRange();

    add(take());
}

void Substitutor::replaceRange() {
    Token* leftSign = (next().has({"+", "-"}))? &take() : nullptr;
    Token left = combineSign(leftSign, take());
    skip();

    Token* rightSign = (next().has({"+", "-"}))? &take() : nullptr;
    Token right = combineSign(rightSign, take());

    if(not right.of({left.type})) { 
        throw Fault::fail(right, fmt::format("Expecting {}", 
            Token::typeToString(left.type)));
    }

    if(not next().has({"by"})) {
        if(left.type == Token::NUM) return replaceIntRange(left, right);
        return replaceCharRange(left, right);
    }

    skip();

    Token* sign = (next().has({"+", "-"}))? &take() : nullptr;
    Token stepToken = combineSign(sign, take());
    
    if(not stepToken.of({Token::NUM}) or not stepToken.isInt()) 
        throw Fault::fail(stepToken, "Expecting Integer");
        
    long long step = std::stoll(stepToken.string);
    if(step == 0) throw Fault::fail(stepToken, "Expecting non-zero step size");

    right.locale[1] = stepToken.locale[1]; 
    
    if(left.type == Token::NUM) return replaceIntRange(left, right, &step);
    return replaceCharRange(left, right, &step);
}

void Substitutor::replaceIntRange(Token left, Token right, long long* stepPtr) {
    if(not left.isInt()) 
        throw Fault::fail(left, "Expecting Integer");
    
    if(not right.isInt())
        throw Fault::fail(right, "Expecting Integer");

    long long leftNum = std::stoll(left.string);
    long long rightNum = std::stoll(right.string);

    SourceLine& line = left.line;
    Token::Locale locale = {left.locale[0], right.locale[1]};

    long long step = stepPtr? *stepPtr : ((leftNum < rightNum)? 1 : -1);

    while(true) {
        if(leftNum < 0) add({line, Token::OP, "-", locale});
        str string = fmt::format("{}", std::abs(leftNum));
        add({line, Token::NUM, string, locale});

        if(leftNum == rightNum) break;
        leftNum += step;

        if(step < 0 and leftNum < rightNum) break;
        if(step > 0 and leftNum > rightNum) break;

        add({line, Token::PUNC, ",", locale});
    }
}

void Substitutor::replaceCharRange(Token left, Token right, long long* stepPtr) {
    if(left.string[0] == '\\' and charMap.find(left.string) == charMap.end()) 
        throw Fault::fail(left, "Invalid escape character");

    if(right.string[0] == '\\' and charMap.find(right.string) == charMap.end()) 
        throw Fault::fail(right, "Invalid escape character");

    char leftChar = (left.string[1] == '\\')?  charMap[left.string] : left.string[1];
    char rightChar = (right.string[1] == '\\')?  charMap[right.string] : right.string[1];

    fmt::print("'{}', '{}'\n", leftChar, rightChar);

    SourceLine& line = left.line;
    Token::Locale locale = {left.locale[0], right.locale[1]};

    long long step = stepPtr? *stepPtr : ((leftChar < rightChar)? 1 : -1);

    while(true) {
        str string = (strMap.find(leftChar) != strMap.end())? strMap[leftChar] 
            : fmt::format("'{}'", leftChar);

        add({line, Token::CHAR, string, locale});

        if(leftChar == rightChar) break;
        leftChar += step;

        if(step < 0 and leftChar < rightChar) break;
        if(step > 0 and leftChar > rightChar) break;

        add({line, Token::PUNC, ",", locale});
    }
}
