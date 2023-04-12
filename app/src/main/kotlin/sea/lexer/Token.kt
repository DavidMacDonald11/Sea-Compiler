package sea.lexer

import sea.Faults
import sea.lexer.Locale
import sea.lexer.SourceLine

data class Token(val line: SourceLine, var type: Type) : Faults.Component {
    public enum class Type(val data: Pair<Char, String>) { 
        PUNC(Pair('P', "Puncuator")), NUM(Pair('N', "Number")), 
        OP(Pair('O', "Operator")), CHAR(Pair('C', "Character")), 
        STR(Pair('S', "String")), IDENTIFIER(Pair('I', "Integer")), 
        KEYWORD(Pair('K', "Keyword")), C_TOKENS(Pair('#', "C Tokens")), 
        NONE(Pair('?', "None"));

        val label: Char 
            get() = data.first

        val word: String 
            get() = data.second
    }

    var string: String
    var locale: Locale

    init {
        locale = line.newLocale()
        string = line.string.substring(locale[0], locale[1])
    }

    fun of(vararg types: Type ): Boolean = type in types
    fun has(vararg strings: String): Boolean = (if(string == "") "EOF" else string) in strings
    fun isInt(): Boolean = of(Type.NUM) && !("." in string)
    
    override fun toString(): String {
        var string = if(string == "") "EOF" else string.replace("\n", "\\n")
        return "${type.label}'$string'"
    }

    override fun tree(prefix: String): String = toString()
    override fun lines(): ArrayList<SourceLine> = ArrayList(listOf(line))
    override fun raw(): String = line.raw()
    override fun mark() = line.mark(this)

    companion object {
        val PREFIX_UNARY_OPS = setOf("+", "-", "!")
        val BINARY_OPS = setOf("*", "/", "+", "-", "<<", ">>", "&", "$", "|")
        val COMPARE_OPS = setOf("<", ">", "<=", ">=", "==", "!=")
        val ASSIGN_OPS = setOf("*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "$=", "|=", "=")
        val OPERATORS = PREFIX_UNARY_OPS + BINARY_OPS + COMPARE_OPS + ASSIGN_OPS

        val PUNC_SYMBOLS = "{}[](),;#%"
        val NUMBER_SYMBOLS = "0123456789"
        val OPERATOR_SYMBOLS = OPERATORS.flatMap { it.asIterable() }.toSet()
        val IDENTIFIER_START_SYMBOLS = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        val IDENTIFIER_SYMBOLS = IDENTIFIER_START_SYMBOLS + "0123456789"

        val PRIMARY_KEYWORDS = setOf("true", "false")
        val KEYWORDS = PRIMARY_KEYWORDS + setOf("not", "and", "xor", "or", "mod", "as", "if", "else")

        val LINE_ENDS = setOf(";", "\n", "EOF")
    }
}
