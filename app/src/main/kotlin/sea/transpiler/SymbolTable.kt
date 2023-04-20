package sea.transpiler

import kotlin.reflect.KClass
import sea.transpiler.symbols.*

typealias SClass = KClass<Symbol>

class SymbolTable(val transpiler: Transpiler) {
    var parent: SymbolTable? = null
    val symbols = mutableMapOf<String, Pair<SClass, Symbol>>()
    val num: Int

    init {
        num = count
        count += 1
    }

    override fun toString(): String {
        if(parent == null) return "$num $symbols"

        return "$parent,\n    $num $symbols"
    }

    private fun newSymbol(name: String, cls: SClass, symbol: Symbol): Symbol {
        if(name in symbols.keys) {
            val node = transpiler.context.node!!
            transpiler.faults.error(node, "Cannot declare identifier '$name' twice")
        }

        symbols.put(name, Pair(cls, symbol))
        return symbol
    }

    fun newVal(name: String, type: TType): Symbol {
        if(type.nullable) {
            val node = transpiler.context.node!!
            transpiler.faults.error(node, "Cannot declare nullable value")
        }

        @Suppress("UNCHECKED_CAST")
        return newSymbol(name, Value::class as SClass, Value(type, name))
    }

    fun newVar(name: String, type: TType): Symbol {
        @Suppress("UNCHECKED_CAST")
        return newSymbol(name, Variable::class as SClass, Variable(type, name))
    }

    fun newInvar(name: String, type: TType): Symbol {
        @Suppress("UNCHECKED_CAST")
        return newSymbol(name, Invariable::class as SClass, Invariable(type, name))
    }

    companion object {
        var count = 0
    }
}
