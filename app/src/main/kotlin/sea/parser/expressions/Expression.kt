package sea.grammar

import sea.parser.*
import sea.grammar.PrimaryExpression

abstract class Expression : Node() {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return PrimaryExpression.construct(parser)
        }
    }
}
