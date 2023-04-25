package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Invariable(type: TType, name: String, storage: String?): Variable(type, name, storage) {
    override val cName = "__sea_invar_${name}__"
    override val cValName = "__sea_invar_${name}_value__"

    override fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!

        if(initialized)
            transpiler.faults.error(node, "Cannot reassign invariable")

        return super.assign(transpiler, expression)
    }

    override fun transfer(transpiler: Transpiler, expression: TExpression, value: Value): TExpression {
        val node = transpiler.context.node!!

        if(value !is Invariable)
            transpiler.faults.error(node, "Cannot transfer/borrow invar into var")

        return super.transfer(transpiler, expression, value)
    }
}
