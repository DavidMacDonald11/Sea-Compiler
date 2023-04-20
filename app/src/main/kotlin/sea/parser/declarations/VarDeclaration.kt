package sea.grammar

import sea.parser.*
import sea.transpiler.symbols.*

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

            (symbol as Value).declare(transpiler, expression)
        }
    }
}
