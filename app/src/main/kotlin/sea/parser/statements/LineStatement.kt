package sea.grammar

import sea.parser.*
import sea.grammar.Expression
import sea.grammar.LineStatementPart

data class LineStatement(val statement: Node) : Node() {
    override val parts: Parts = listOf(statement)    

    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val node = LineStatementPart.construct(parser)
            parser.expectingHas(*Parser.lineEnds)

            return LineStatement(node)
        }
    }
}
