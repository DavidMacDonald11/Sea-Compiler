package sea.transpiler

import kotlin.math.pow
import sea.transpiler.OutputFile

fun createStandard(outDir: String) {
    val file = OutputFile(outDir, "__sea_std__.h")

    file.include("limits")
    file.include("stddef")
    file.include("stdint")
    file.include("stdbool")

    for(i in 4..6) {
        val bits = 2.toDouble().pow(i).toInt()
        file.alias("uint_least${bits}_t", "Nat$bits")
        file.alias("int_least${bits}_t", "Int$bits")
    }

    file.alias("bool", "Bool")
    file.alias("uint_least8_t", "Byte")
    file.alias("char", "Char")
    file.alias("unsigned int", "Nat")
    file.alias("int", "Int")
    file.alias("float", "Real32")
    file.alias("float", "Imag32")
    file.alias("double", "Real")
    file.alias("double", "Imag")
    file.alias("long double", "Real64")
    file.alias("long double", "Imag64")
    file.alias("_Complex float", "Cplex32")
    file.alias("_Complex double", "Cplex")
    file.alias("_Complex long double", "Cplex64")

    file.write("""
        |
        |#ifndef __imag__
        |   #define __imag__(X) cimag((X))
        |#endif
        |#ifndef __real__
        |   #define __real__(X) creal((X))
        |#endif
    """.trimMargin())
}
