package sea.lexer

import kotlin.math.pow
import sea.Faults
import sea.lexer.Locale
import sea.lexer.SourceLine

val UPPERCASE_LETTERS = ('A'..'Z').joinToString("")
val LOWERCASE_LETTERS = ('a'..'z').joinToString("")

enum class TokenType(val data: Pair<Char, String>) { 
    PUNC(Pair('P', "Puncuator")), NUM(Pair('N', "Number")), 
    OP(Pair('O', "Operator")), CHAR(Pair('C', "Character")), 
    STR(Pair('S', "String")), IDENTIFIER(Pair('I', "Identifier")), 
    KEYWORD(Pair('K', "Keyword")), NONE(Pair('?', "None"));

    val label: Char 
        get() = data.first

    val word: String 
        get() = data.second
}

data class Token(val line: SourceLine, var type: TokenType) : Faults.Component {
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

    fun of(vararg types: TokenType ): Boolean = type in types
    fun of(c: Collection<TokenType>): Boolean = of(*c.toTypedArray())
    fun has(vararg strings: String): Boolean = (if(string == "") "EOF" else string) in strings
    fun has(c: Collection<String>): Boolean = has(*c.toTypedArray())
    fun isInt(): Boolean = of(TokenType.NUM) && !("." in string)
    
    override fun toString(): String {
        var string = if(string == "") "EOF" else string.replace("\n", "\\n")
        return "${type.label}'$string'"
    }

    override fun tree(prefix: String): String = toString()
    override fun lines(): ArrayList<SourceLine> = ArrayList(listOf(line))
    override fun raw(): String = line.raw()
    override fun mark() = line.mark(this)

    companion object {
        val POSTFIX_OPS = setOf("!!")
        val PREFIX_UNARY_OPS = setOf("+", "-", "!", "&", "$")
        val BINARY_OPS = setOf("*", "/", "%", "+", "-", "<<", ">>", "&", "$", "|")
        val COMPARE_OPS = setOf("<", ">", "<=", ">=", "==", "!=")
        val ASSIGN_OPS = setOf("*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "$=", "|=", "?:=", "=")
        val PUNC_OPS = setOf(":", "::", "->", "?")
        val OPERATORS = POSTFIX_OPS + PREFIX_UNARY_OPS + BINARY_OPS + COMPARE_OPS + ASSIGN_OPS + 
            PUNC_OPS + setOf("?:")

        val PUNC_SYMBOLS = "{}[](),;#@"
        val NUMBER_START_SYMBOLS = "0123456789."
        val NUMBER_SYMBOLS = NUMBER_START_SYMBOLS + "b" + UPPERCASE_LETTERS
        val OPERATOR_SYMBOLS = OPERATORS.flatMap { it.asIterable() }.toSet()
        val IDENTIFIER_START_SYMBOLS = "_" +  UPPERCASE_LETTERS + LOWERCASE_LETTERS
        val IDENTIFIER_SYMBOLS = IDENTIFIER_START_SYMBOLS + "0123456789"

        val PRIMARY_KEYWORDS = setOf("true", "false", "null", "infinity", "nan")
        val VIS_KEYWORDS = setOf("public", "private")
        val STORAGE_KEYWORDS = setOf("static", "cpu")
        val VAR_KEYWORDS = setOf("val", "invar", "var")
        val TYPE_KEYWORDS = setOf("Bool", "Byte", "Char", 
            "Int16", "Int", "Int32", "Int64",
            "Nat16", "Nat", "Nat32", "Nat64",
            "Real32", "Real", "Real64",
            "Imag32", "Imag", "Imag64",
            "Cplex32", "Cplex", "Cplex64")
        val FUN_KEYWORDS = setOf("fun", "inline", "deviant")
        val KEYWORDS = PRIMARY_KEYWORDS + VAR_KEYWORDS + VIS_KEYWORDS + TYPE_KEYWORDS + 
            FUN_KEYWORDS + setOf(
            "not", "and", "xor", "or", 
            "mod", "as", "if", "else", "while", "do",
            "pass", "return")

        val LINE_ENDS = setOf(";", "\n", "EOF")
    }
}
