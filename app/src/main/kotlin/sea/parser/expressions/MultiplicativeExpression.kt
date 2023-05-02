package sea.grammar

import sea.parser.*

class MultiplicativeExpression(left: Node, op: Token, right: Node)
    : BinaryOperation(left, op ,right) {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            @Suppress("UNCHECKED_CAST")
            val cls = MultiplicativeExpression::class as BinOpClass
            val hasList = listOf("*", "/", "%", "mod")

            return construct(parser, hasList, UnaryExpression::construct, cls)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        if(op.has("%", "mod")) return transpiler.nodeContext(this) {
            transpileRemainder(transpiler)
        }

        return transpiler.nodeContext(this) {
            val left = left.transpile(transpiler).arithmeticOp(transpiler)
            val right = right.transpile(transpiler).arithmeticOp(transpiler)
            val result = TExpression.resolveType(left, right)

            result.replace("$left ${op.string} $right")
        }
    }

    fun transpileRemainder(transpiler: Transpiler): TExpression {
        val left = left.transpile(transpiler).arithmeticOp(transpiler)
        val right = right.transpile(transpiler).arithmeticOp(transpiler)
        val result = TExpression.resolveType(left, right)

        if("Nat" in result.type || "Int" in result.type) {
            return result.replace("$left % $right")
        }

        transpiler.include("tgmath")

        if("Real" in left.type && "Real" in right.type) {
            return result.replace("fmod($left, $right)").notConstant()
        }

        val util = transpiler.includeUtil("cmod", result.type, ::cplexModUtil)
        return result.replace("$util($left, $right)").notConstant()
    }

    fun cplexModUtil(util: String, type: String): String {
        return """
            |$type $util($type a, $type b) {
            |   $type result = a / b;
            |   result = floor(creal(result)) + floor(cimag(result)) * 1.0j;
            |   return a - b * result;
            |}
        """.trimMargin()
    }
}
