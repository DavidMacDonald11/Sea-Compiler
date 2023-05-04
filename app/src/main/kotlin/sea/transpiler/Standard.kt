package sea.transpiler

import kotlin.math.pow
import sea.lexer.Token
import sea.transpiler.OutputFile

fun createStandard(outDir: String) {
    val file = OutputFile(outDir, "__sea_std__.h")

    file.include("limits")
    file.include("stddef")
    file.include("stdint")
    file.include("stdbool")
    file.include("stdlib")
    file.write("")

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
    file.alias("double", "Real")
    file.alias("long double", "Real64")
    file.alias("_Complex float", "Cplex32")
    file.alias("_Complex double", "Cplex")
    file.alias("_Complex long double", "Cplex64")

    createNullableTypes(file)
}

fun createNullableTypes(file: OutputFile) {
    file.write("""
        |typedef struct { bool isNull: 1; __sea_type_Bool__ value: 1; }
        | __sea_type_NullableBool__;
    """.trimMargin().replace("\n", ""))

    for(type in Token.TYPE_KEYWORDS) {
        if(type == "Bool") continue

        file.write("""
            |typedef struct { bool isNull: 1; __sea_type_${type}__ value; }
            | __sea_type_Nullable${type}__;
        """.trimMargin().replace("\n", ""))
    }

    file.write("""
        |typedef struct { bool isNull: 1; void *value; } __sea_type_NullableAny__;
    """.trimMargin())
}
