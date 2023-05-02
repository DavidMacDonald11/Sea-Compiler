package sea.grammar

import sea.parser.*

class TypeName(token: Token): PrimaryNode(token) {
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.next.has("Array")) return ArrayType.construct(parser)
            return TypeName(parser.expectingHas(Token.TYPE_KEYWORDS))
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        return TExpression(TType(token.string))
    }
}

