package sea.transpiler

import sea.Faults
import sea.parser.Node
import sea.transpiler.OutputFile

data class Transpiler(val faults: Faults, val tree: Node, val oFile: OutputFile) {
    var utils = mutableSetOf<String>()
    var lines: String = ""

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
    
    fun includeUtil(util: String, type: String, makeUtil: (String, String) -> String): String {
        val name = "${util}_$type"
        if(name !in utils) header(makeUtil("__sea_util_${name}__", "__sea_type_${type}__"))
        return "__sea_util_${name}__"
    }

    fun transpileTree() {
        tree.transpile(this)
        oFile.write(lines, "")
    }
}


data class TExpression(var type: String = "", var string: String = "") {
    private var showType = false
    private var finished = false
    var dynamic: Boolean = false
    var nullable: Boolean = false

    override fun toString() = "$string"
    fun setShowtype(): TExpression { showType = true; return this }

    fun replace(string: String): TExpression { 
        this.string = string 
        return this 
    }

    fun cast(type: String): TExpression {
        this.type = type
        return this
    }

    fun castReplace(type: String): TExpression {
        this.type = type.replaceFirst("\\w+", type)
        return this
    }

    fun castUp(): TExpression {
        if(type in listOf("Bool", "Byte", "Char")) type = "Nat16"
        return this
    }

    fun dropImag(transpiler: Transpiler): TExpression {
        if("Imag" !in type) return this

        transpiler.include("tgmath")
        return add("cimag(", ")")
    }

    fun isReal(): Boolean {
        return "Imag" !in type && "Cplex" !in type && type != "None"
    }

    fun add(before: String = "", after: String = ""): TExpression { 
        string = "$before$string$after"
        return this
    }

    fun arithmeticOp(node: Node, transpiler: Transpiler): TExpression {
        if(type == "None") transpiler.faults.error(node, "Cannot operate on None type")
        return this.castUp()
    }

    fun finish(transpiler: Transpiler, semicolons: Boolean = true): TExpression {
        if(finished) return this
        finished = true

        val indent = "" // TODO finish
        var end = if(semicolons) ";" else ""

        var printType = "$type"
        if(dynamic) printType = "#$printType"
        if(nullable) printType = "$printType?"

        if(showType) end = "$end /*$printType*/"
        return add(indent, "$end\n")
    }

    companion object {
        val typeValues = mapOf(
            "Any" to 100.0, "None" to 0.0,
            "Bool" to 1.0, "Byte" to 1.5, "Char" to 2.0,
            "Nat16" to 3.0, "Int16" to 3.5,
            "Nat" to 4.0, "Int" to 4.5,
            "Nat32" to 5.0, "Int32" to 5.5,
            "Nat64" to 6.0, "Int64" to 6.5,
            "Real32" to 7.0, "Imag32" to 7.0,
            "Real" to 8.0, "Imag" to 8.0,
            "Real64" to 9.0, "Imag64" to 9.0,
            "Cplex32" to 10.0, "Cplex" to 11.0, "Cplex64" to 12.0
        )

        fun resolveType(left: TExpression, right: TExpression): TExpression {
            val lType = left.type
            val rType = right.type

            val lValue = typeValues[lType]?: 0.0
            val rValue = typeValues[rType]?: 0.0

            return TExpression(if(lValue >= rValue) lType else rType)
        }

        fun realAndImag(left: TExpression, right: TExpression): Boolean {
            if(left.isReal() && "Imag" in right.type) return true
            return "Imag" in left.type && right.isReal()
        }
    }
}
