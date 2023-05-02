package sea.grammar

import sea.parser.*

data class CastExpression(val expression: Node, val type: Node): Node() {
    override val parts: Parts = listOf(expression, type)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            var node = AdditiveExpression.construct(parser)

            while(parser.next.has("as")) {
                parser.take()
                node = CastExpression(node, Type.construct(parser))
            }

            return node
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        var result = expression.transpile(transpiler)

        if("None" in result.type.string) {
            transpiler.faults.error(this, "Cannot cast from 'None' type")
            return result
        }

        val type = type.transpile(transpiler).type
        val cType = type.cName
        val rawCType = type.rawCName

        if(type.dynamic) result.type.dynamic = true
        if(result.type == type) return result

        if(result.isNull) {
            if(!type.nullable)
                transpiler.faults.error(this, "Cannot cast null to non-nullable type")

            return result.cast(type)
        }

        if(!result.type.nullable) {
            if(!type.nullable) return result.add("($rawCType)(", ")").cast(type)
            return result.add("($cType){false, ($rawCType)(", ")}").cast(type)
        }

        return result.add("__sea_macro_castNullable__($rawCType, ", ")").cast(type)
    }
}
