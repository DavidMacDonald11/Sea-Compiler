package sea.grammar

import sea.parser.*
import sea.transpiler.symbols.Symbol
import sea.transpiler.symbols.Variable

data class VarDeclaration(val visibility: Token?, val varKeyword: Token, val declarator: Node): Node() {
    override val parts: Parts = listOf(visibility, varKeyword, declarator)
    
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val visibility = if(parser.next.has(Token.VIS_KEYWORDS)) parser.take() else null
            val varKeyword = parser.expectingHas(Token.VAR_KEYWORDS)
            val declarator = VarDeclarator.construct(parser)

            if(varKeyword.has("val")) {
                if((declarator as VarDeclarator).expression == null) {
                    parser.faults.error(declarator, "Value must be initialized at construction")
                }
            }

            return VarDeclaration(visibility, varKeyword, declarator)
        }
    }

    override fun publish(publisher: Publisher) {
        if(!(visibility?.has("public")?: false)) return
        publisher.map.put((declarator as VarDeclarator).identifier.string, this)
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        return transpiler.nodeContext(this) {
            val name = (declarator as VarDeclarator).identifier.string

            val given = declarator.type?.transpile(transpiler)?.type
            val expression = declarator.expression?.transpile(transpiler)
            val type = TType.resolveAssign(transpiler, given, expression?.type)

            val symbol = when(varKeyword.string) {
                "val" -> transpiler.symbols.newVal(name, type)
                "var" -> transpiler.symbols.newVar(name, type)
                else -> transpiler.symbols.newInvar(name, type)
            }

            expression?.dropImag(transpiler)
            val cName = symbol.cName

            if(varKeyword.has("val")) {
                val decl = "#define"
                val initial = expression!!.add(" ")

                val result = TExpression(type, "$decl $cName")
                result.add(after = initial.string).setShowType().finish(transpiler, false)
            } else {
                var cType = type.cName
                val initial = expression ?: TExpression()
                var result = TExpression(type, "$cType $cName")

                if(type.nullable) {
                    val cValName = (symbol as Variable).cValName
                    result.replace("$cType $cValName")

                    if(initial.type.nullable) {
                        val expr = initial.string

                        if(initial.isValue) {
                            result.new(type, "$cType *$cName = NULL").setShowType()
                        } else {
                            result = result.new(type, "$cType *$cName = ($expr)? ($cValName = *$expr")
                            result.add(after = ", &$cValName) : NULL").setShowType()
                        }
                    } else {
                        if(expression != null) initial.add(" = ")
                        result.add(after = initial.string)
                        result.new(type, "$cType *$cName = &$cValName").setShowType()
                    }
                } else {
                    if(expression != null) initial.add(" = ")
                    result.add(after = initial.string).setShowType()
                }
            }
        }
    }
}
