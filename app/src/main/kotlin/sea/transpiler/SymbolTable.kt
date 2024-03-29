package sea.transpiler

import sea.transpiler.symbols.*

class SymbolTable(val transpiler: Transpiler) {
    var parent: SymbolTable? = null
    val symbols = mutableMapOf<String, Symbol>()
    val num: Int

    val isGlobal get() = (parent == null)

    init {
        num = count
        count += 1
    }

    operator fun get(key: String): Symbol {
        val found = symbols[key]

        if(found != null) return found
        if(parent != null) return parent!![key]

        val node = transpiler.context.node!!
        throw transpiler.faults.fail(node, "Reference to undeclared identifier '$key'")
    }

    override fun toString(): String {
        if(parent == null) return "$num $symbols"

        return "$parent,\n    $num $symbols"
    }

    private fun newSymbol(name: String, symbol: Symbol): Symbol {
        if(name in symbols.keys) {
            val node = transpiler.context.node!!
            transpiler.faults.error(node, "Cannot declare identifier '$name' twice")
        }

        symbols.put(name, symbol)
        return symbol
    }

    fun newVal(name: String, type: TType): Symbol = newSymbol(name, Value(type, name))
    fun newVar(name: String, type: TType, storage: String?) = newSymbol(name, Variable(type, name, storage))
    fun newInvar(name: String, type: TType, storage: String?) = newSymbol(name, Invariable(type, name, storage))

    companion object {
        var count = 0
    }
}
