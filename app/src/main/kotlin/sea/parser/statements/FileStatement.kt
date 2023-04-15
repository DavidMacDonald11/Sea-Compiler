package sea.grammar

import sea.parser.*
import sea.grammar.LineStatement
import sea.publisher.Publisher

class FileStatement : Node() {
    val statements = ArrayList<Node>()

    override val parts: Parts
        get() = (statements.map { it })

    fun makeTree(parser: Parser) {
        while(!parser.next.has("EOF")) {
            statements.add(LineStatement.construct(parser))
        }
    }

    override fun publish(publisher: Publisher) {
        statements.forEach { it.publish(publisher) }
    }
}
