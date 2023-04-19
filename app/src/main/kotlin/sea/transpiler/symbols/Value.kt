package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Value(type: TType, name: String): Symbol(type, name) {
    override val cName = "__sea_val_${name}__"

    open fun access(transpiler: Transpiler): TExpression {
        return TExpression(type, cName)
    }
}
