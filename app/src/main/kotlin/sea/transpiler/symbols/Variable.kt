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
        initialized = (expression != null)

        val cType = verifyStorage(transpiler, expression)
    
        if(type.dynamic) {
            transpiler.include("stdlib")
            var result = TExpression(type, "$cType *$cName = malloc(sizeof($cType))")
            
            if(!type.nullable || expression == null || !expression.type.nullable) {
                if(expression == null) {
                    if(type.nullable) return TExpression(type, "$cType *$cName").setShowType()
                    return result.setShowType()
                }

                result.setShowType()
                result = result.new(string = "*$cName = ").add(after = expression.string)
                
                return result
            }

            if(expression.isConstant) 
                return TExpression(type, "$cType *$cName = NULL").setShowType()

            if(expression.type.dynamic) expression.add("&(", ")")
            var expr = expression.string

            if(expression.delayedCast == null) {
                val setVal = "$cName = (malloc(sizeof($cType)), *$cName = *$expr, $cName)"
                expr = "($expr)? ($setVal) : NULL"
            } else {
                val name = expression.delayedCast!!.string
                val setVal = "$cName = (malloc(sizeof($cType)), *$cName = $expr, $cName)"
                expr = "($name)? ($setVal) : NULL"
            }

            return result.replace("$cType *$cName = $expr").setShowType()
        }

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

        if(expression.type.dynamic) expression.add("&(", ")")
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

        val eType = type.copy()
        eType.dynamic = true

        val expression = TExpression(eType, "$cName")

        if(type.dynamic) expression.add("(*", ")")
        if("Imag" in type) expression.add("(", " * 1.0j)")

        expression.isConstant = false
        if(initialized) return expression

        transpiler.faults.error(node, "Cannot access uninitialized identifier '$name'")
        return expression
    }

    private fun verifyStorage(transpiler: Transpiler, expression: TExpression?): String {
        val node = transpiler.context.node!!
        var storage = storage?.plus(" ") ?: ""
        val cType = type.cName

        if("Cplex" !in type) expression?.dropImag()

        if(transpiler.symbols.isGlobal) {
            if(this.storage != "static")
                //transpiler.faults.error(node, "Global variables must be static")
            else storage = ""
        }

        if(this.storage == "static" && expression?.isConstant?.not() ?: false) 
            transpiler.faults.error(node, "Initial value of static variable must be constant")
        
        if(this.storage == "cpu") {
            if(type.nullable) 
                transpiler.faults.error(node, "Cannot declare nullable cpu variable")
            
            if(type.dynamic)
                transpiler.faults.error(node, "Cannot declare dynamic cpu variable")
        }
                    
        return storage.replace("cpu", "register") + "$cType"
    }
}
