package sea.lexer

import java.io.File
import sea.lexer.SourceLine

data class SourceFile(val path: String) {
    val lines: ArrayList<SourceLine> = ArrayList()

    init {
        File(path).bufferedReader().useLines { fileLines ->
            for((i, line) in fileLines.withIndex()) {
                lines.add(SourceLine(i + 1, "$line\n"))
            }
        }

        lines.add(SourceLine(lines.size + 1, ""))
    }

    var line: SourceLine = lines[0]

    val atEnd: Boolean
        get() = line.num > lines.size - 1

    val next: Char
        get() = if(line.unreadString != "") line.unreadString[0] else '\u0000'

    fun iterateLine() {
        if(!atEnd) line = lines[line.num]
    }

    fun take(num: Int = -1, these: String = "", until: String = ""): String {
        if(atEnd || num == 0) return ""
        var string = ""

        for(c in line.unreadString) {
            if(c in until || (these != "" && !(c in these))) return string

            string += c
            line.increment()

            if(string.length == num) break
        }

        if(line.unreadString == "") iterateLine()
        return string
    } 
}
