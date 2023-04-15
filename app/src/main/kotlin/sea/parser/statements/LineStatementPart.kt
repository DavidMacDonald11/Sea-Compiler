package sea.grammar

import sea.parser.*
import sea.grammar.Expression

data class LineStatementPart(val part: Node) : Node() {
    override val parts: Parts = listOf(part)    

    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val node = Expression.construct(parser)
            return LineStatementPart(node)
        }
    }
}
