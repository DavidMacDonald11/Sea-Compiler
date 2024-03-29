package sea.grammar

import sea.parser.*
import sea.grammar.MultiplicativeExpression

class AdditiveExpression(left: Node, op: Token, right: Node)
    : BinaryOperation(left, op, right) {
        companion object : Node.CompanionObject {
            override fun construct(parser: Parser): Node {
                @Suppress("UNCHECKED_CAST")
                val cls = AdditiveExpression::class as BinOpClass
                val hasList = listOf("+", "-")

                return construct(parser, hasList, MultiplicativeExpression::construct, cls)
            }
        }

        override fun transpile(transpiler: Transpiler): TExpression {
            return transpiler.nodeContext(this) {
                val left = left.transpile(transpiler).arithmeticOp(transpiler)
                val right = right.transpile(transpiler).arithmeticOp(transpiler)
                val result = TExpression.resolveType(left, right)

                if(left.longValue != null && right.longValue != null) {
                    if(op.has("+")) result.longValue = left.longValue!! + right.longValue!!
                    else result.longValue = left.longValue!! - right.longValue!!
                }

                result.replace("$left ${op.string} $right")
            }
         }
    }
