package sea.parser

import sea.Faults
import sea.lexer.SourceLine
import sea.lexer.Token
import sea.parser.Parser
import kotlin.reflect.KClass
import kotlin.reflect.full.primaryConstructor

typealias Parts = List<Faults.Component>

abstract class Node : Faults.Component {
    abstract val parts: Parts
    override fun toString(): String = tree("    ")

    override fun tree(prefix: String): String {
        var string = "${this::class.simpleName}"
        
        for((i, node) in parts.withIndex()) {
            val atLast = (i == parts.size - 1)
            val symbol = if(atLast) "└──" else "├──"

            val children = node.tree("$prefix${if(atLast) "" else "│"}    ")
            string += "\n$prefix$symbol $children"
        }

        return string
    }

    override fun lines(): List<SourceLine> {
        val lines = mutableSetOf<SourceLine>()

        for(c in parts) {
            lines += c.lines().toSet()
        }

        return lines.toList().sortedBy { it.num }
    }


    override fun raw(): String {
        var string = "" 
        for(line in lines()) string += line.raw()
        return string
    }

    override fun mark() {
        for(c in parts) c.mark()
    }

    interface CompanionObject {
        fun construct(parser: Parser): Node?
    }
}


abstract class PrimaryNode(var token: Token) : Node() {
    override val parts: Parts = listOf(token)
    override fun tree(prefix: String): String = "${this::class.simpleName} ── $token"
}


private typealias ConstructFun = (Parser) -> Node
private typealias BinOpClass = KClass<BinaryOperation>

abstract class BinaryOperation(var left: Node, var op: Token, var right: Node) : Node() {
    override val parts: Parts = listOf(left, op, right)

    companion object {
        fun construct(parser: Parser, hasList: List<String>, makeChild: ConstructFun, Type: BinOpClass): Node {
                var node = makeChild(parser)

                while(parser.next.has(*hasList.toTypedArray())) {
                    val op = parser.take()
                    val right = makeChild(parser)
                    node = Type.primaryConstructor!!.call(node, op, right)
                }

                return node
        }
    }
}
