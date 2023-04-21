package sea.transpiler

import sea.transpiler.symbols.*

class SymbolTable(val transpiler: Transpiler) {
    var parent: SymbolTable? = null
    val symbols = mutableMapOf<String, Symbol>()
    val num: Int

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

    fun newVal(name: String, type: TType): Symbol {
        if(type.nullable) {
            val node = transpiler.context.node!!
            transpiler.faults.error(node, "Cannot declare nullable value")
        }

        return newSymbol(name, Value(type, name))
    }

    fun newVar(name: String, type: TType) = newSymbol(name, Variable(type, name))
    fun newInvar(name: String, type: TType) = newSymbol(name, Invariable(type, name))

    companion object {
        var count = 0
    }
}
