package sea.transpiler

import java.io.File

data class OutputFile(val outDir: String, val srcPath: String) {
    var path: String
    var headers = mutableSetOf<String>()

    init {
        path = srcPath.replace(".sea", ".c")
        path = File(path).name
        path = "$outDir/$path"

        File(path).writeText("")
    }

    fun write(string: String, end: String = "\n") {
        File(path).appendText("$string$end")
    }

    fun alias(cName: String, seaName: String) {
        write("typedef $cName __sea_type_${seaName}__;")
    }

    fun include(header: String, lib: Boolean = true) {
        val cs = if(lib) Pair('<', '>') else Pair('"', '"')
        if(headers.add(header)) write("#include ${cs.first}$header.h${cs.second}")
    }
}
