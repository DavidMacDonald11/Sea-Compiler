package sea.grammar

import sea.parser.*
import sea.grammar.Expression
import sea.publisher.Publisher

data class LineStatementPart(val part: Node) : Node() {
    override val parts: Parts = listOf(part)    

    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val node = Expression.construct(parser)
            return LineStatementPart(node)
        }
    }

    override fun publish(publisher: Publisher) {
        part.publish(publisher)
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        return part.transpile(transpiler).setShowtype()
    }
}
