package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Value(type: TType, name: String): Symbol(type, name) {
    override val cName = "__sea_val_${name}__"

    open fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        val initial = expression!!.dropImag().add(" ")

        val node = transpiler.context.node!!
        if(!initial.isConstant) transpiler.faults.error(node, "Can only create constant values")

        val result = TExpression(type, "#define $cName").add(after = initial.string)
        return result.setShowType().finish(transpiler, false)
    }

    open fun access(transpiler: Transpiler): TExpression {
        return TExpression(type, cName)
    }
}
