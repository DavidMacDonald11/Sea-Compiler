package sea.transpiler

import sea.transpiler.TType
import sea.transpiler.Transpiler

data class TExpression(var type: TType = TType(), var string: String = "") {
    private var showType = false
    private var finished = false
    private var previous = ArrayList<TExpression>()
    var transfer: Pair<String, String>? = null
    var isConstant = true
    var isNull = false

    constructor(type: String, string: String = ""): this(TType(type), string)

    override fun toString(): String {
        val result = previous.joinToString(separator = "")
        return "$result$string"
    }

    fun notConstant(): TExpression {
        isConstant = false
        return this
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
    }
}
