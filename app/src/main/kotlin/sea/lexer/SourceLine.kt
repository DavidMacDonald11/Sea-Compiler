package sea.lexer

import sea.lexer.Locale

data class SourceLine(val num: Int, val string: String) {
    private var locale: Locale = Locale()
    val marks: ArrayList<Locale> = ArrayList()

    var unreadString = ""
        get() = string.substring(locale[1])

    fun ignore() { locale[0] = locale[1] }
    fun mark(locale: Locale) { marks.add(locale) }
    fun increment() { if(locale[1] < string.length) locale[1] += 1 }

    fun newLocale(): Locale {
        val result = locale.copy()
        locale[0] = locale[1]
        return result
    }

    fun raw(): String {
        var col1 = 0
        var col2 = 0

        for(locale in marks) {
            if(col1 == 0 || locale[0] < col1) col1 = locale[0]
            if(col2 == 0 || locale[1] > col2) col2 = locale[1]
        }

        var underline = ""

        for(i in string.indices) {
            underline += if(i < col1 || i >= col2) ' ' else '^'
        }

        val line = if(string == "") "EOF\n" else string
        if(string == "") underline = "^^^"

        return "%4d|$line    |$underline".format(num)
    }
}
