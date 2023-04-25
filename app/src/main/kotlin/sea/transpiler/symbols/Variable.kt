package sea.transpiler.symbols

import sea.parser.Node
import sea.transpiler.Transpiler
import sea.transpiler.TType
import sea.transpiler.TExpression

open class Variable(type: TType, name: String, val storage: String?): Value(type, name) {
    var initialized = false
    var transfered = false
    var transfer: String? = null

    override val cName = "__sea_var_${name}__"
    open val cValName = "__sea_var_${name}_value__"

    override fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        initialized = (expression != null)
        val cType = verifyStorage(transpiler, expression)

        if(expression?.transfer != null) return declareTransfer(transpiler, expression, cType)
        if(type.dynamic) return declareDynamic(transpiler, expression, cType)
        return declareNonDynamic(expression, cType)
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

            if(expression?.transfer != null)
                transpiler.faults.error(node, "Cannot transfer/borrow into cpu variable")
        }

        return storage.replace("cpu", "register") + "$cType"
    }

    private fun declareTransfer(transpiler: Transpiler, expression: TExpression, cType: String): TExpression {
        transfer = expression.transfer!!.first

        val transfer = expression.transfer!!
        val variable = transpiler.symbols[transfer.second]
        val expr = (variable as Value).transfer(transpiler, expression, this)

        return expr.add("$cType *$cName = ").setShowType()
    }

    private fun declareDynamic(transpiler: Transpiler, expression: TExpression?, cType: String): TExpression {
        transpiler.include("stdlib")
        var result = TExpression(type, "$cType *$cName = malloc(sizeof($cType))").setShowType()

        if(!type.nullable || expression == null || !expression.type.nullable) {
            if(expression == null) {
                if(type.nullable) return result.replace("$cType *$cName")
                return result
            }

            return result.new(string = "*$cName = ").add(after = expression.string)
        }

        if(expression.isConstant)
            return result.replace("$cType *$cName = NULL")

        var expr = expression.string

        if(expression.delayedCast == null) {
            val setVal = "$cName = (malloc(sizeof($cType)), *$cName = *$expr, $cName)"
            expr = "($expr)? ($setVal) : NULL"
        } else {
            val name = expression.delayedCast!!.string
            val setVal = "$cName = (malloc(sizeof($cType)), *$cName = $expr, $cName)"
            expr = "($name)? ($setVal) : NULL"
        }

        return result.replace("$cType *$cName = $expr")
    }

    private fun declareNonDynamic(expression: TExpression?, cType: String): TExpression {
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

        var expr = expression.string

        if(expression.delayedCast == null) {
            expr = "($expr)? ($cValName = *$expr, &$cValName) : NULL"
        } else {
            val name = expression.delayedCast!!.string
            expr = "($name)? ($cValName = $expr, &$cValName) : NULL"
        }

        return result.new(type, "$cType *$cName = $expr").setShowType()
    }

    override fun access(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!

        if(transfered)
            transpiler.faults.error(node, "Cannot use dead identifier after ownership transfer")

        val eType = type.copy()
        eType.dynamic = type.dynamic

        val expression = TExpression(eType, "$cName")

        if(!type.nullable && (type.dynamic || transfer != null)) expression.add("(*", ")")
        if("Imag" in type) expression.add("(", " * 1.0j)")

        expression.isConstant = false
        if(initialized) return expression

        transpiler.faults.error(node, "Cannot access uninitialized identifier '$name'")
        return expression
    }

    open fun assign(transpiler: Transpiler, expression: TExpression): TExpression {
        val node = transpiler.context.node!!

        if(transfered)
            transpiler.faults.error(node, "Cannot use dead identifier after ownership swap")

        TType.resolveAssign(transpiler, type, expression.type)

        initialized = true
        return expression.dropImag()
    }

    override fun transfer(transpiler: Transpiler, expression: TExpression, value: Value): TExpression {
        val node = transpiler.context.node!!
        transfered = (expression.transfer?.first == "$")

        if(type != value.type)
            transpiler.faults.error(node, "Cannot transfer/borrow into variable with different type")

        if(storage == "cpu")
            transpiler.faults.error(node, "Cannot transfer/borrow cpu variable")

        return if(!value.type.nullable) expression.add("&") else expression
    }
}
