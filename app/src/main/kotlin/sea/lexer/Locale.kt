package sea.lexer

data class Locale(var x: Int = 0, var y: Int = 0) {
    operator fun get(index: Int): Int = if(index % 2 == 0) x else y

    operator fun set(index: Int, value: Int) {
        if(index % 2 == 0) x = value
        else y = value
    }
}
