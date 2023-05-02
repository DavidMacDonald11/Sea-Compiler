package sea.transpiler

import sea.transpiler.TType
import sea.transpiler.Transpiler

data class TExpression(var type: TType = TType(), var string: String = "") {
    private var showType = false
    private var finished = false
    private var previous = ArrayList<TExpression>()
    var transfer: Pair<String, String>? = null
    var isConstant = true

    val isNull get() = isConstant && type.nullable

    constructor(type: String, string: String = ""): this(TType(type), string)

    override fun toString(): String {
        val result = previous.joinToString(separator = "")
        return "$result$string"
    }

    fun replace(string: String): TExpression {
        this.string = string
        return this
    }

    fun cast(type: TType): TExpression {
        this.type = type
        return this
    }

    fun cast(type: String): TExpression {
        this.type.string = type
        return this
    }

    fun castReplace(type: String): TExpression {
        this.type.string = this.type.string.replaceFirst("[a-zA-Z]+".toRegex(), type)
        return this
    }

    fun castUp(): TExpression {
        if(type.string in listOf("Bool", "Byte", "Char")) type.string = "Nat16"
        return this
    }

    fun dropImag(): TExpression {
        if("Imag" !in type) return this
        return add("((", ") / 1j)")
    }

    fun isReal(): Boolean {
        return "Imag" !in type && "Cplex" !in type && type.string != "None"
    }

    fun add(before: String = "", after: String = ""): TExpression {
        string = "$before$string$after"
        return this
    }

    fun arithmeticOp(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!

        if(type.string == "None") transpiler.faults.error(node, "Cannot operate on None type")
        if(type.nullable) transpiler.faults.error(node, "Cannot operate on nullable type")
        if(transfer != null) transpiler.faults.error(node, "Cannot operate on ownership expression")

        return this.castUp()
    }

    fun new(type: TType = TType(), string: String = ""): TExpression {
        val expression = this.copy()
        previous.add(expression)
        return cast(type).replace(string)
    }

    // fun store(): TExpression {
    //     val name = "__sea_temp_${temps++}__"
    //     return add("#define $name ").new(type, name)
    // }

    // fun memStore(): TExpression {
    //     val const = if(isConstant) "const " else ""
    //     val cType = type.cName
    //     val name = "__sea_temp_${temps++}__"

    //     return add("$const$cType $name = ").new(type, name)
    // }

    fun mergePrevious(vararg expressions: TExpression) {
        expressions.forEach {
            previous.addAll(it.previous)
        }
    }

    fun setShowType(): TExpression { showType = true; return this }

    fun finish(transpiler: Transpiler, semicolons: Boolean = true): TExpression {
        if(finished) return this
        finished = true

        previous.forEach { it.finish(transpiler, semicolons) }

        val indent = "    ".repeat(transpiler.context.indents)
        var end = if(semicolons) ";" else ""

        if(showType) end = "$end /*$type*/"
        return add(indent, "$end\n")
    }

    companion object {
        var temps = 0

        fun resolveType(left: TExpression, right: TExpression): TExpression {
            val result = TExpression(TType.resolve(left.type, right.type).copy())
            result.isConstant = left.isConstant && right.isConstant
            return result
        }

        fun realAndImag(left: TExpression, right: TExpression): Boolean {
            if(left.isReal() && "Imag" in right.type) return true
            return "Imag" in left.type && right.isReal()
        }
    }
}
