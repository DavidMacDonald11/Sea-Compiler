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

    override fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        initialized = (expression == null)

        val cType = type.cName
        expression?.dropImag(transpiler)
        
        if(!type.nullable) {
            val result = TExpression(type, "$cType $cName")
            if(expression != null) result.add(after = expression.add(" = ").string)
            return result.setShowType()
        }

        var result = TExpression(type, "$cType $cValName")
        
        if(expression == null || !expression.type.nullable) {
            if(expression != null) result.add(after = expression.add(" = ").string)
            return result.new(type, "$cType *$cName = &$cValName")
        }

        if(expression.isConstant) 
            return result.new(type, "$cType *$cName = NULL").setShowType()

        val expr = expression.string
        result = result.new(type, "$cType *$cName = ($expr)? ($cValName = *$expr")
        return result.add(after = ", &$cValName) : NULL").setShowType()
    }

    open fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")
    
        initialized = true
        return expression.dropImag(transpiler)
    }

    override fun access(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")

        val expression = TExpression(type, "*cName")
        if(initialized) return expression

        transpiler.faults.error(node, "Cannot access uninitialized identifier '$name'")
        return expression
    }
}
