package sea.transpiler.symbols

import sea.transpiler.TType

abstract class Symbol(val type: TType, val name: String) {
    abstract val cName: String

    override fun toString() = cName
}
