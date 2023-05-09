package sea.grammar

import sea.parser.*
import sea.transpiler.symbols.*

data class VarDeclaration(val visibility: Token?, val storage: Token?, val varKeyword: Token, val declarator: Node): Node() {
    override val parts: Parts = listOf(visibility, storage, varKeyword, declarator)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val visibility = if(parser.next.has(Token.VIS_KEYWORDS)) parser.take() else null
            val storage = if(parser.next.has(Token.STORAGE_KEYWORDS)) parser.take() else null
            val varKeyword = parser.expectingHas(Token.VAR_KEYWORDS)
            val declarator = VarDeclarator.construct(parser)

            if(varKeyword.has("val")) {
                if(storage != null) {
                    parser.faults.error(storage, "Value cannot have storage modifier")
                }

                if((declarator as VarDeclarator).expression == null) {
                    parser.faults.error(declarator, "Value must be initialized at construction")
                }
            }

            return VarDeclaration(visibility, storage, varKeyword, declarator)
        }
    }

    override fun publish(publisher: Publisher) {
        if(!(visibility?.has("public")?: false)) return
        publisher.map.put((declarator as VarDeclarator).identifier.string, this)
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        if(transpiler.symbols.isGlobal && visibility != null) {
            transpiler.faults.error(this, "Can only specify visibility in the global scope")
        }

        return transpiler.nodeContext(this) {
            val name = (declarator as VarDeclarator).identifier.string
            val given = declarator.type?.transpile(transpiler)?.type

            transpiler.assignContext(given) {
                val expression = declarator.expression?.transpile(transpiler)
                var type = TType.resolveAssign(transpiler, given, expression?.type)

                val symbol = when(varKeyword.string) {
                    "val" -> transpiler.symbols.newVal(name, type)
                    "var" -> transpiler.symbols.newVar(name, type, storage?.string)
                    else -> transpiler.symbols.newInvar(name, type, storage?.string)
                }

                (symbol as Value).declare(transpiler, expression)
            }
        }
    }
}
