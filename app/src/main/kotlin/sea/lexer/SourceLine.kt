package sea.lexer

import sea.lexer.Locale
import sea.lexer.Token

data class SourceLine(val num: Int, val string: String) {
    private var locale = Locale()
    val marks = ArrayList<Locale>()

    val unreadString: String
        get() = string.substring(locale[1])

    fun ignore() { locale[0] = locale[1] }
    fun mark(locale: Locale) { marks.add(locale) }
    fun mark(token: Token) { marks.add(token.locale) }
    fun increment() { if(locale[1] < string.length) locale[1] += 1 }

    fun newLocale(): Locale {
        val result = locale.copy()
        locale[0] = locale[1]
        return result
    }

    fun raw(): String {
        var col1 = -1
        var col2 = -1

        for(locale in marks) {
            println(locale)
            if(col1 == -1 || locale[0] < col1) col1 = locale[0]
            if(col2 == -1 || locale[1] > col2) col2 = locale[1]
        }

        var underline = ""

        for(i in string.indices) {
            underline += if(i < col1 || i >= col2) ' ' else '^'
        }

        val line = if(string == "") "EOF\n" else string
        if(string == "") underline = "^^^"

        return "${"%4d".format(num)}|$line    |$underline"
    }
}
