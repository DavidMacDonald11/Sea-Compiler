package sea.transpiler

import sea.transpiler.Transpiler

data class TType(var string: String = "Any", var dynamic: Boolean = false, var nullable: Boolean = false) {
    val cName: String get() {
        if(nullable) return "__sea_type_Nullable${if(string == "Array") "Any" else string}__"
        return if(string == "Array") "${arrayType!!.cName}" else "__sea_type_${string}__"
    }

    var arrayType: TType? = null
    var arraySize: Long? = null

    val arrayDim: Int get() = if(arrayType != null) arrayType!!.arrayDim + 1 else 0

    fun copy(): TType {
        val result = TType(string, dynamic, nullable)
        result.arrayType = arrayType?.copy()
        result.arraySize = arraySize
        return result
    }

    override fun toString(): String {
        var result = string

        if(result == "Array") {
            val inner = arrayType!!.toString()
            result = "[${arraySize!!}: ${inner}]"
        }

        if(dynamic) result = "#$result"
        if(nullable) result = "$result?"

        return result
    }

    operator fun contains(item: String) = item in string

    companion object {
        val typeValues = mapOf(
            "Any" to 0.0, "None" to 100.0,
            "Bool" to 1.0, "Byte" to 1.5, "Char" to 2.0,
            "Nat16" to 3.0, "Int16" to 3.5,
            "Nat" to 4.0, "Int" to 4.5,
            "Nat32" to 5.0, "Int32" to 5.5,
            "Nat64" to 6.0, "Int64" to 6.5,
            "Real32" to 7.0, "Real" to 8.0, "Real64" to 9.0,
            "Cplex32" to 10.0, "Cplex" to 11.0, "Cplex64" to 12.0,
            "Array" to 20.0
        )

        fun resolve(left: TType, right: TType): TType {
            if("Array" in left && "Array" in right) {
                val result = resolve(left.arrayType!!, right.arrayType!!)
                return if(result === left.arrayType!!) left else right
            }

            val lValue = typeValues[left.string]?: 0.0
            val rValue = typeValues[right.string]?: 0.0

            return if(lValue >= rValue) left else right
        }

        fun resolveSizes(transpiler: Transpiler, given: TType, inferred: TType) {
            if("Array" !in given) return

            val node = transpiler.context.node!!

            if(given.arraySize!! > inferred.arraySize!!) {
                transpiler.faults.error(node, "Too few elements in assign of $inferred to $given")
                return
            }

            if(given.arraySize!! < inferred.arraySize!!) {
                transpiler.faults.warn(node, "Too many elements in assign of $inferred to $given")
            }

            resolveSizes(transpiler, given.arrayType!!, inferred.arrayType!!)
        }

        fun resolveAssign(transpiler: Transpiler, given: TType?, inferred: TType?): TType {
            if(inferred == null) return given!!
            val node = transpiler.context.node!!

            if("None" in inferred) {
                transpiler.faults.error(node, "Cannot assign to None-type value")
                return given ?: inferred
            }

            if(given == null) {
                if("Any" in inferred) transpiler.faults.error(node, "Cannot infer type")
                return inferred
            }

            if(!given.nullable && inferred.nullable) {
                val nType = "nullable type"
                transpiler.faults.error(node, "Cannot assign $nType $inferred to non-$nType $given")
                return given
            }

            if(given.nullable && inferred.nullable && "Any" in inferred) {
                return given
            }

            if(resolve(given, inferred) != given || "Array" in given && "Array" !in inferred) {
                transpiler.faults.error(node, "Cannot assign type $inferred to type $given")
                return given
            }

            val i = inferred.arrayDim
            val g = given.arrayDim

            if(i != g) {
                transpiler.faults.error(node, "Cannot assign $i-D array to $g-D array")
                return given
            }

            resolveSizes(transpiler, given, inferred)
            return given
        }
    }
}
