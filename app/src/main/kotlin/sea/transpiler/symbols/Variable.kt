package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Variable(type: TType, name: String): Value(type, name) {
    var initialized = false
    var transfered = false

    override val cName = "__sea_var_${name}__"
    open val cValName = "__sea_var_${name}_value__"

    override fun access(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")

        val expression = TExpression(type, "*cName")
        if(initialized) return expression

        transpiler.faults.error(node, "Cannot access uninitialized identifier '$name'")
        return expression
    }

    open fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")
    
        initialized = true
        return expression.dropImag(transpiler)
    }
}
