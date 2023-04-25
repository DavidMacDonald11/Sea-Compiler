package sea.grammar

import sea.parser.*
import sea.publisher.Publisher

data class LineStatementPart(val part: Node): Node() {
    override val parts: Parts = listOf(part)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.nextOrAfterHas(Token.VAR_KEYWORDS + Token.STORAGE_KEYWORDS)) {
                return LineStatementPart(VarDeclaration.construct(parser))
            }

            return LineStatementPart(Expression.construct(parser))
        }
    }

    override fun publish(publisher: Publisher) {
        part.publish(publisher)
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = part.transpile(transpiler)

        if(part is OwnershipExpression)
            transpiler.faults.error(part, "Must transfer/borrow into variable or function")

        if(part !is VarDeclaration) result.dropImag().setShowType()

        return result
    }
}
