package sea

import kotlinx.coroutines.*
import java.util.concurrent.CountDownLatch
import sea.Faults
import sea.lexer.SourceFile
import sea.lexer.SourceLine
import sea.lexer.Locale
import sea.lexer.Lexer

class App {}

fun main(args: Array<String>) = runBlocking {
    val options = args[0]
    val outDir = args[1]
    val filePaths = args.drop(2)

    val jobs = ArrayList<Job>(filePaths.size)
    val latch = CountDownLatch(jobs.size)

    for(filePath in filePaths) {
        val job = launch { compileFile(options, filePath, outDir, latch) }
        jobs.add(job)
    }

    jobs.joinAll()
}

suspend fun compileFile(options: String, srcPath: String, outDir: String, latch: CountDownLatch) {
    val sourceFile = SourceFile(srcPath)
    val faults = Faults()
    
    val lexer = Lexer(faults, sourceFile)
    if(runLexer(lexer)) return
    println(lexer)

    // publish

    latch.countDown()
    latch.await()

    // transpile
}

fun runLexer(lexer: Lexer): Boolean = runStage(lexer.faults) { 
    println("Building ${lexer.file.path}...")
    lexer.makeTokens()
}

fun runStage(faults: Faults, func: () -> Unit): Boolean {
    var failed = false

    try {
        func()
        faults.finishStage()
    } catch(e: Faults.CompilerFailure) {
        failed = true
        println("$faults")
    }

    return failed
}
