package sea.lexer

import java.io.File
import sea.lexer.SourceLine

data class SourceFile(val filePath: String) {
    val lines: ArrayList<SourceLine> = ArrayList()

    init {
        File(filePath).bufferedReader().useLines { fileLines ->
            for((i, line) in fileLines.withIndex()) {
                lines.add(SourceLine(i + 1, "$line\n"))
            }
        }
    }

    var line: SourceLine? = lines[0]

    fun iterateLine() {
        if(line != null) 
            line = if(line!!.num < lines.size) lines[line!!.num] else null
    }
    
    fun next(): Char = if(line?.unreadString == "") line!!.unreadString[0] else '\u0000'

    fun take(num: Int = -1, these: String = "", until: String = ""): String {
        var string: String = ""

        if(line == null || num == 0) return string

        for(c in line!!.unreadString) {
            if(c in until || (these != "" && !(c in these))) return string

            string += c
            line!!.increment()

            if(string.length == num) break
        }

        if(line!!.unreadString == "") iterateLine()
        return string
    } 
}
