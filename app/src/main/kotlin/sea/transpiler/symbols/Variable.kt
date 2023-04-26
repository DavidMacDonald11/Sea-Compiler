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

    override fun declare(transpiler: Transpiler, expression: TExpression?): TExpression {
        initialized = (expression != null)
        val s = verifyStorage(transpiler, expression)
        if(expression?.transfer != null) return declareTransfer(transpiler, expression, s)

        val cType = type.cName
        var result = TExpression(type, "$s$cType ")

        if(!type.dynamic) result.add(after = "$cName").setShowType()
        else result.add(after = "*$cName = malloc(sizeof($cType))")

        if(expression == null) return result
        if(type.dynamic) result = result.new(type, "*$cName").setShowType()

        val expr = expression.string
        val eType = expression.type
        val typesMatch = (type.nullable == eType.nullable && type.string == eType.string)

        if(expression.isNull) return result.add(after = " = ($cType){true}")
        if(!type.nullable || typesMatch) return result.add(after = " = $expr")
        if(!eType.nullable) return result.add(after = " = ($cType){false, $expr}")

        val rawCType = type.rawCName
        return result.add(after = " = ($cType){$expr.isNull, ($rawCType)$expr.value}")
    }

    private fun verifyStorage(transpiler: Transpiler, expression: TExpression?): String {
        val node = transpiler.context.node!!
        var storage = storage?.plus(" ") ?: ""

        if("Cplex" !in type) expression?.dropImag()

        if(transpiler.symbols.isGlobal) {
            if(this.storage != "static")
                //transpiler.faults.error(node, "Global variables must be static")
            else storage = ""
        }

        if(this.storage == "static" && expression?.isConstant?.not() ?: false)
            transpiler.faults.error(node, "Initial value of static variable must be constant")

        return storage
    }

    private fun declareTransfer(transpiler: Transpiler, expression: TExpression, s: String): TExpression {
        val cType = type.cName
        transfer = expression.transfer!!.first

        val transfer = expression.transfer!!
        val variable = transpiler.symbols[transfer.second]
        val expr = (variable as Value).transfer(transpiler, expression, this)

        return expr.add("$s$cType *$cName = ").setShowType()
    }

    override fun access(transpiler: Transpiler): TExpression {
        val node = transpiler.context.node!!

        if(transfered)
            transpiler.faults.error(node, "Cannot use dead identifier after ownership transfer")

        val eType = type.copy()
        eType.dynamic = type.dynamic

        val expression = TExpression(eType, "$cName")

        if(type.dynamic || transfer != null) expression.add("(*", ")")
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

        if((value as Variable).storage == "static" && storage != "static")
            transpiler.faults.error(node, "Cannot transfer/borrow non-static var into static var")

        return if(!value.type.nullable) expression.add("&") else expression
    }
}
