package sea.lexer

import sea.Faults
import sea.lexer.SourceFile
import sea.lexer.Token
import sea.lexer.TokenType

data class Lexer(val faults: Faults, val file: SourceFile) {
    val tokens = ArrayList<Token>()

    override fun toString(): String = tokens.joinToString(", ", "[", "]")

    fun newToken(type: TokenType = TokenType.NONE, line: SourceLine = file.line): Token {
        val token = Token(line, type)
        tokens.add(token)
        return token
    }

    fun makeTokens() {
        do makeToken() while(!file.atEnd)
    }

    fun makeToken() {
        ignoreSpaces()

        when(file.next) {
            '\\' -> return makeLineContinuation()
            '\u0000', in Token.PUNC_SYMBOLS -> return makePunctuator()
            '\'' -> return makeChar()
            '\u0022' -> return makeString()
            in Token.NUMBER_START_SYMBOLS -> return makeNumber()
            in Token.OPERATOR_SYMBOLS -> return makeOperator()
            in Token.IDENTIFIER_START_SYMBOLS -> return makeIdentifier()
        }

        file.take(1)
        val token = newToken()

        throw faults.fail(token, "Unrecognized symbol '${token.string}'")
    }

    fun ignoreSpaces() {
        while(file.next.isWhitespace()) {
            if(file.next == '\n') {
                makeNewline()
                continue
            }

            file.take(1)
            file.line.ignore()
        }
    }

    fun ignoreComment() {
        if(file.take(1) == "/") {
            file.take(until = "\n")
            return file.line.ignore()
        }

        val line = file.line

        while(file.next != '\u0000') {
            if(file.next != '*') {
                file.take(until = "*")
                continue
            }
            
            file.take(1)

            if(file.next == '/') {
                file.take(1)
                return file.line.ignore()
            }
        }

        throw faults.fail(newToken(line = line), "Unterminated multiline comment")
    }

    fun makeNewline() {
        val line = file.line

        while(file.next == '\n') {
            file.take(1)
        }

        newToken(TokenType.PUNC, line)
    }

    fun makeLineContinuation() {
        file.take(1)

        while(file.next.isWhitespace()) {
            if(file.take(1) == "\n") return file.line.ignore()
        }

        file.line.ignore()
        file.take(until = "\n")

        throw faults.fail(newToken(), "Unexpected symbols after line continuation character")
    }

    fun makePunctuator() {
        file.take(1)
        newToken(TokenType.PUNC)
    }

    fun makeChar() {
        val pattern = """'(\\[btnr'"\\$]|\\u[a-fA-F0-9]{4}|[^\\'])'""".toRegex()
        file.take(1)

        if(file.next == '\\') {
            file.take(1)

            if(file.next == '\'') file.take(1)
            file.take(until = "'")
        } else file.take(1)

        if(file.take(1) == "'") {
            val token = newToken(TokenType.CHAR)
            if(token.string.matches(pattern)) return

            faults.error(token, "Invalid character")
            return
        }

        throw faults.fail(newToken(), "Unterminated character")
    }

    fun makeString() {
        file.take(1)

        while(file.next != '\n') {
            val taken = file.take(1)
            if(taken == "\\") file.take(1)

            if(taken == "\"") {
                newToken(TokenType.STR)
                return
            }
        }

        throw faults.fail(newToken(), "Unterminated string")
    }

    fun makeNumber() {
        var pattern = """(\d+)|(\d*\.\d+)|(\d+\.\d*)|"""
        pattern += """(\d+b(([0-9A-Z]+)|([0-9A-Z]*\.[0-9A-Z]+)|([0-9A-Z]+\.[0-9A-Z]*)))"""

        if(file.take(1) == "." && !(file.next.isDigit())) {
            newToken(TokenType.OP)
            return
        }

        file.take(these = Token.NUMBER_SYMBOLS)
        val token = newToken(TokenType.NUM)

        if(!token.string.matches(pattern.toRegex())) throw faults.fail(token, "Invalid number")
        token.convertNumber(faults)
    }

    fun makeOperator() {
        var string = ""
        var next: String

        checkNextSym@ while(true) {
            next = string + file.next

            for(op in Token.OPERATORS) {
                if(next in op) {
                    string += file.take(1)
                    continue@checkNextSym
                }
            }

            break
        }

        if(string == "/" && file.next in "/*") return ignoreComment()
        val token = newToken(if(string in Token.PUNC_OPS) TokenType.PUNC else TokenType.OP)

        if(string !in Token.OPERATORS) {
            throw faults.fail(token, "Unrecognized operator '${token.string}'")
        }
    }

    fun makeIdentifier() {
        val string = file.take(these = Token.IDENTIFIER_SYMBOLS)
        newToken(if(string in Token.KEYWORDS) TokenType.KEYWORD else TokenType.IDENTIFIER)
    }
}
