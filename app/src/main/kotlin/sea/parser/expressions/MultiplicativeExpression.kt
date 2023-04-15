package sea.grammar

import sea.parser.*
import sea.grammar.PrimaryExpression

class MultiplicativeExpression(left: Node, op: Token, right: Node) 
    : BinaryOperation(left, op ,right) {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            @Suppress("UNCHECKED_CAST")
            val cls = MultiplicativeExpression::class as BinOpClass
            val hasList = listOf("*", "/", "%", "mod")

            return construct(parser, hasList, PrimaryExpression::construct, cls)
        }
    }
}
