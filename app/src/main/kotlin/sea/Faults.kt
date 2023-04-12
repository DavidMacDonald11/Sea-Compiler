package sea

import sea.lexer.SourceLine

class Faults {
    class CompilerFailure : Exception() {}

    var stage = 0
    var warnings = ArrayList<String>()
    var errors = ArrayList<String>()
    var failure: String? = null

    companion object {
        val stageMap = arrayOf("Lexing", "Parsing", "Publishing", "Transpiling")
    }

    override fun toString(): String {
        var string = ""

        string += warnings.joinToString("\n")
        string += errors.joinToString("\n")
        string += failure ?: ""

        return string
    }

    fun warn(c: Component, message: String) = warnings.add(act(c, message, "Warning"))
    fun error(c: Component, message: String) = errors.add(act(c, message, "Error"))
    
    fun fail(c: Component, message: String): CompilerFailure {
        failure = act(c, message, "Failure")
        return CompilerFailure()
    }

    fun finishStage() {
        stage += 1
        if(warnings.size + errors.size == 0) return
        throw CompilerFailure()
    }

    fun act(c: Component, message: String, label: String): String {
        c.mark()

        val stageLabel = "${stageMap[stage]} $label"
        var newMessage = message.replace("\n", "\\n")

        return "$stageLabel: $newMessage\n${c.raw()}"
    }

    public interface Component {
        override fun toString(): String
        fun tree(prefix: String): String
        fun lines(): ArrayList<SourceLine>
        fun raw(): String
        fun mark()
    }
}
