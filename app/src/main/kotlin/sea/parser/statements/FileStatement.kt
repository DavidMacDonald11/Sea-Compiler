package sea.grammar

import sea.parser.*
import sea.grammar.LineStatement

class FileStatement : Node() {
    val statements = ArrayList<Node>()

    override val parts: Parts
        get() = (statements.map { it })

    fun makeTree(parser: Parser) {
        while(!parser.next.has("EOF")) {
            statements.add(LineStatement.construct(parser))
        }
    }
}
