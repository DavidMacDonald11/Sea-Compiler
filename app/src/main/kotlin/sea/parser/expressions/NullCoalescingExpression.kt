package sea.grammar

import sea.parser.*

class NullCoalescingExpression(left: Node, op: Token, right: Node)
    : BinaryOperation(left, op ,right) {
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            @Suppress("UNCHECKED_CAST")
            val cls = NullCoalescingExpression::class as BinOpClass
            return construct(parser, listOf("?:"), CastExpression::construct, cls)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val left = left.transpile(transpiler)
        val right = right.transpile(transpiler)

        if(!left.type.nullable) {
            op.mark()
            transpiler.faults.warn(this.right, "Non-nullable value on left is always the result")
            return left
        }

        if(left.isNull) {
            op.mark()
            transpiler.faults.warn(this.left, "'null' value on left is never the result")
            return right
        }

        var name = left.string
        val result = TExpression.resolveType(left, right).replace("($name).isNull? ")
        result.type.nullable = right.type.nullable

        if(!result.type.nullable) return result.add(after = "(${right.string}) : ($name).value")

        val cType = result.type.cName
        val rawCType = result.type.rawCName

        if(right.isNull) return result.add(after = "($cType){true} : ($name)")

        if(left.type.string != result.type.string) {
            left.add("__sea_macro_castNullable__($rawCType, ", ")")
        }

        if(right.type.string != result.type.string) {
            if(!right.type.nullable) right.add("($cType){false, ", "}")
            else right.add("__sea_macro_castNullable__($rawCType, ", ")")
        }

        return result.add(after = "(${right.string}) : (${left.string})")
    }
}
