package sea.lexer

import kotlin.math.pow
import sea.Faults
import sea.lexer.Locale
import sea.lexer.SourceLine

val UPPERCASE_LETTERS = ('A'..'Z').joinToString("")
val LOWERCASE_LETTERS = ('a'..'z').joinToString("")

data class Token(val line: SourceLine, var type: Type) : Faults.Component {
    public enum class Type(val data: Pair<Char, String>) { 
        PUNC(Pair('P', "Puncuator")), NUM(Pair('N', "Number")), 
        OP(Pair('O', "Operator")), CHAR(Pair('C', "Character")), 
        STR(Pair('S', "String")), IDENTIFIER(Pair('I', "Integer")), 
        KEYWORD(Pair('K', "Keyword")), NONE(Pair('?', "None"));

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

    fun convertNumber(faults: Faults) {
        if("b" !in string) return

        val base = string.substring(0, string.indexOf("b")).toDouble()
        if(base < 2) throw faults.fail(this, "Numeric base must be at least 2")

        val num = string.substring(string.indexOf("b") + 1)
        
        var (int, frac) = num.split(".").let { Pair(it[0], it.getOrElse(1){ "" }) }
        var result = 0.0

        for((i, c) in int.withIndex()) {
            val digit = if(c.isDigit()) c - '0' else c - 'A' + 10
            if(digit >= base) throw faults.fail(this, "Invalid base ${base.toInt()} digit")
            result += digit * base.pow(int.length - i - 1)
        }

        for((i, c) in frac.withIndex()) {
            val digit = if(c.isDigit()) c - '0' else c - 'A' + 10
            if(digit >= base) throw faults.fail(this, "Invalid base ${base.toInt()} digit")
            result += digit * base.pow(-(i + 1))
        }

        string = if("." in num) "$result" else "${result.toInt()}"
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
        val NUMBER_START_SYMBOLS = "0123456789."
        val NUMBER_SYMBOLS = NUMBER_START_SYMBOLS + "b" + UPPERCASE_LETTERS
        val OPERATOR_SYMBOLS = OPERATORS.flatMap { it.asIterable() }.toSet()
        val IDENTIFIER_START_SYMBOLS = "_" +  UPPERCASE_LETTERS + LOWERCASE_LETTERS
        val IDENTIFIER_SYMBOLS = IDENTIFIER_START_SYMBOLS + "0123456789"

        val PRIMARY_KEYWORDS = setOf("true", "false")
        val KEYWORDS = PRIMARY_KEYWORDS + setOf("not", "and", "xor", "or", "mod", "as", "if", "else")

        val LINE_ENDS = setOf(";", "\n", "EOF")
    }
}
