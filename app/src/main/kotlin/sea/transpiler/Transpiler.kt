package sea.transpiler

import sea.Faults
import sea.parser.Node

class Context {
    var nodes = ArrayList<Node>()
    var indents = 0
    var assignType: TType? = null

    val node get() = if(nodes.size == 0) null else nodes[nodes.size - 1]
}


data class Transpiler(val faults: Faults, val tree: Node, val oFile: OutputFile) {
    var utils = mutableSetOf<String>()
    var lines: String = ""
    var symbols = SymbolTable(this)
    val context = Context()

    override fun toString(): String = "$symbols"

    init {
        include("__sea_std__", lib = false)
    }

    fun header(string: String = "", end: String = "\n") {
        oFile.write("$string$end")
    }

    fun write(string: String = "", end: String = "\n") {
        lines += "$string$end"
    }

    fun include(header: String, lib: Boolean = true) = oFile.include(header, lib)

    fun includeUtil(util: String, type: TType, makeUtil: (String, String) -> String): String {
        val name = "${util}_$type"
        if(name !in utils) header(makeUtil("__sea_util_${name}__", "__sea_type_${type}__"))
        return "__sea_util_${name}__"
    }

    fun transpileTree() {
        tree.transpile(this)
        oFile.write(lines, "")
    }

    fun nodeContext(node: Node, func: () -> TExpression): TExpression {
        context.nodes.add(node)
        val result = func()
        context.nodes.remove(node)
        return result
    }

    fun assignContext(type: TType?, func: () -> TExpression): TExpression {
        context.assignType = type
        val result = func()
        context.assignType = null
        return result
    }
}
