package sea

import java.io.File

data class DebugFile(val outDir: String, val srcPath: String, val options: String) {
    var path: String

    val enabled: Boolean
        get() = "d" in options

    init {
        path = srcPath + "debug"
        path = File(path).name
        path = "$outDir/$path"

        if(enabled) File(path).writeText("$srcPath\n\n")
    }

    fun write(string: String, end: String = "\n\n") {
        if(enabled) File(path).appendText("$string$end")
    }
}
