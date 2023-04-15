package sea.parser

import sea.Faults
import sea.lexer.Token
import sea.grammar.FileStatement

data class Parser(val faults: Faults, val tokens: ArrayList<Token>) {
    var i = 0
    var tree: Node = FileStatement()

    override fun toString() = "$tree"

    fun makeTree() {
        (tree as FileStatement).makeTree(this)
    }

    val next: Token
        get() = tokens[if(i > tokens.size - 1) tokens.size - 1 else i]

    fun ahead(pos: Int): Token {
        i = this.i + pos
        return tokens[if(i > tokens.size - 1) tokens.size - 1 else i]
    }

    fun take(): Token {
        val token = next
        i += 1
        return token
    }

    fun expectingOf(vararg types: Token.Type): Token {
        if(next.of(*types)) return take()

        val names = (types.map { it.name }).joinToString(", ", "[", "]")
        throw faults.fail(take(), "Expecting type of $names")
    }

    fun expectingHas(vararg values: String): Token {
        if(next.has(*values)) return take()

        val vals = values.joinToString(", ", "[", "]")
        throw faults.fail(take(), "Expecting value of $vals")
    }

    fun skipNewlines() {
        while(next.has("\n")) take()
    }

    companion object {
        val lineEnds = Token.LINE_ENDS.toTypedArray()
    }
}
