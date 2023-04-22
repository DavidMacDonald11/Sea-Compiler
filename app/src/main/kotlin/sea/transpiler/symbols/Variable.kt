package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Variable(type: TType, name: String, val storage: String?): Value(type, name) {
    var initialized = false
    var transfered = false

    override val cName = "__sea_var_${name}__"
    open val cValName = "__sea_var_${name}_value__"

    override fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        val node = transpiler.context.node!!
        initialized = (expression != null)

        var cType = type.cName
        if("Cplex" !in type) expression?.dropImag()

        var storage = storage?.plus(" ") ?: ""

        if(transpiler.symbols.isGlobal) {
            if(this.storage != "static")
                //transpiler.faults.error(node, "Global variables must be static")
            else storage = ""
        }

        if(this.storage == "static" && expression?.isConstant?.not() ?: false) {
            transpiler.faults.error(node, "Initial value of static variable must be constant")
        }

        if(this.storage == "cpu" && type.nullable) {
            transpiler.faults.error(node, "Cannot declare nullable cpu variable")
        }

        cType = storage.replace("cpu", "register") + "$cType"

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

        if(expression.delayedCast == null) {
            result = result.new(type, "$cType *$cName = ($expr)? ($cValName = *$expr")
        } else {
            val name = expression.delayedCast!!.string
            result = result.new(type, "$cType *$cName = ($name)? ($cValName = $expr")
        }

        return result.add(after = ", &$cValName) : NULL").setShowType()
    }

    open fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")
    
        TType.resolveAssign(transpiler, type, expression.type)

        initialized = true
        return expression.dropImag()
    }

    override fun access(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!
        
        if(transfered) 
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")

        val expression = TExpression(type.copy(), "$cName")
        if("Imag" in type) expression.add("(", " * 1.0j)")
        expression.isConstant = false

        if(initialized) return expression

        transpiler.faults.error(node, "Cannot access uninitialized identifier '$name'")
        return expression
    }
}
