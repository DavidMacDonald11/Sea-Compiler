package sea.grammar

import sea.parser.*
import sea.grammar.AdditiveExpression

abstract class Expression : Node() {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return AdditiveExpression.construct(parser)
        }
    }
}
