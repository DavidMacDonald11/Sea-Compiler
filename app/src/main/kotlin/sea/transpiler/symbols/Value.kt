package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Value(type: TType, name: String): Symbol(type, name) {
    override val cName = "__sea_val_${name}__"

    open fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        val node = transpiler.context.node!!
        val initial = expression!!.dropImag().add(" ")

        if(initial.type.nullable) transpiler.faults.error(node, "Cannot declare nullable value")
        if(initial.type.dynamic) transpiler.faults.error(node, "Cannot declare dynamic value")
        if(initial.transfer != null) transpiler.faults.error(node, "Cannot transfer/borrow into value")
        if(!initial.isConstant) transpiler.faults.error(node, "Value must be constant")

        val result = TExpression(type, "#define $cName").add(after = initial.string)
        return result.setShowType().finish(transpiler, false)
    }

    open fun access(transpiler: Transpiler): TExpression {
        return TExpression(type, cName)
    }

    open fun transfer(transpiler: Transpiler, expression: TExpression, value: Value): TExpression {
        val node = transpiler.context.node!!
        transpiler.faults.error(node, "Cannot transfer/borrow value")
        return TExpression(type, cName)
    }

    open fun free(transpiler: Transpiler): TExpression {
        return TExpression("None", "#undef $cName").finish(transpiler, false)
    }
}
