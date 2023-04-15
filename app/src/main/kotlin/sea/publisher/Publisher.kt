package sea.publisher

import sea.Faults
import sea.parser.Node

data class Publisher(val faults: Faults, val tree: Node) {
    val map = mapOf<String, Node>()

    override fun toString(): String = "$tree"

    fun publishTree() {
        tree.publish(this)
    }
}
