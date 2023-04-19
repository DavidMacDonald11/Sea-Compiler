package sea.grammar

import sea.parser.*

data class VarDeclarator(val identifier: Token, val type: Node?, val expression: Node?): Node() {
    override val parts: Parts = listOf(identifier, type, expression)
    
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val identifier = parser.expectingOf(TokenType.IDENTIFIER)
            var type: Node? = null
            var expression: Node? = null

            if(parser.next.has(":")) {
                parser.take()
                type = Type.construct(parser)
            }

            if(parser.next.has("=")) {
                parser.take()
                expression = Expression.construct(parser)
            }

            if(type == null && expression == null) {
                parser.faults.error(identifier, "Declaration requires a type or an initial value")
            }

            return VarDeclarator(identifier, type, expression)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        throw NotImplementedError()
    }
}

