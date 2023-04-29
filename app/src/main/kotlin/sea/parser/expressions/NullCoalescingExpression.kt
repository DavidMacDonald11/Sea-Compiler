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

        if(TExpression.realAndImag(left, right)) result.castReplace("Cplex")
        if(!result.type.nullable) return result.add(after = "(${right.string}) : ($name).value")

        val cType = result.type.cName
        if(right.isNull) return result.add(after = "($cType){true} : ($name)")

        var newLeft = left
        var newRight = right

        if(left.type.string != result.type.string) {
            name = newLeft.string
            if(newLeft.type.nullable) newLeft.add("(", ").value")
            if("Imag" in newLeft.type) newLeft.add("(", ") * 1j")

            newLeft = CastExpression.castValue(newLeft, result.type)
            newLeft.add("($cType){($name).isNull, ", "}")
        }

        if(right.type.string != result.type.string) {
            name = newRight.string
            if(newRight.type.nullable) newRight.add("(", ").value")
            if("Imag" in newRight.type) newRight.add("(", ") * 1j")

            newRight = CastExpression.castValue(newRight, result.type)
            newRight.add("($cType){($name).isNull, ", "}")
        }

        return result.add(after = "(${newRight.string}) : (${newLeft.string})")
    }
}
