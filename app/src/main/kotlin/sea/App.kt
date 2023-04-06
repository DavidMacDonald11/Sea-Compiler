package sea

import sea.lexer.SourceFile
import sea.lexer.SourceLine
import sea.lexer.Locale

class App {}

fun main() {
    var file = SourceFile("../LICENSE")

    for(line in file.lines) {
        println("${line.raw()}")
    }
}
