package sea.publisher

import sea.Faults
import sea.parser.Node

data class Publisher(val faults: Faults, val tree: Node) {
    val map = mutableMapOf<String, Node>()

    override fun toString(): String {
        var string = ""

        for((key, value) in map) {
            string += "$key:\n\t${value.tree("       ")}\n\t"
        }

        return "$string\n\t$tree"
    }

    fun publishTree() {
        tree.publish(this)
    }

    fun importTree() {
        
    }
}
