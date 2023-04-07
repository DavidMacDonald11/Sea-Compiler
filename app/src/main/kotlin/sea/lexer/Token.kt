package sea.lexer

import sea.lexer.Locale
import sea.lexer.SourceLine

data class Token(val line: SourceLine, var type: Type) {
    enum class Type(val data: Pair<Char, String>) { 
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
        string = line.string.substring(locale[0], locale[1] - locale[0])
    }

    fun of(vararg types: Type ): Boolean = type in types
    fun has(vararg strings: String): Boolean = (if(string == "") "EOF" else string) in strings
    
    override fun toString(): String {
        var string = if(string == "") "EOF" else string.replace("\n", "\\n")
        return "${type.label}'$string'"
    }

    fun tree(): String = toString()
}
