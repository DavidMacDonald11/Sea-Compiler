package sea.grammar

import sea.parser.*

abstract class Expression : Node() {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return CastExpression.construct(parser)
        }
    }
}
