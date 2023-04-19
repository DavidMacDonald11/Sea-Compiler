package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Invariable(type: TType, name: String): Variable(type, name) {
    override val cName = "__sea_invar_${name}__"

    override fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!
        
        if(initialized)
            transpiler.faults.error(node, "Cannot reassign invariable")

        return super.assign(transpiler, expression)
    }
}
