package sea.lexer

import sea.Faults
import sea.lexer.SourceFile
import sea.lexer.Token

data class Lexer(val faults: Faults, val file: SourceFile) {
    val tokens = ArrayList<Token>()

    override fun toString(): String = tokens.joinToString(", ", "[", "]")

    fun newToken(type: Token.Type, line: SourceLine): Token {
        val token = Token(line, type)
        tokens.add(token)
        return token
    }

    fun newToken(type: Token.Type): Token = newToken(type, file.line)
    fun newToken(): Token = newToken(Token.Type.NONE)

    fun makeTokens() {
        do makeToken() while(!file.atEnd)
    }

    fun makeToken() {
        ignoreSpaces()

        when(file.next) {
            '\u0000', in Token.PUNC_SYMBOLS -> return makePunctuator()
            '\\' -> return makeLineContinuation()
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

    fun makeNewline() {
        val line = file.line

        while(file.next == '\n') {
            file.take(1)
        }

        newToken(Token.Type.PUNC, line)
    }

    fun makePunctuator() {
        file.take(1)
        newToken(Token.Type.PUNC)
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
}
