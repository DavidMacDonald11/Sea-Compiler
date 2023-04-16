package sea

import kotlinx.coroutines.*
import java.util.concurrent.CountDownLatch
import sea.Faults
import sea.DebugFile
import sea.lexer.*
import sea.parser.*
import sea.transpiler.OutputFile
import sea.transpiler.createStandard

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

    createStandard(outDir)
    jobs.joinAll()
}

suspend fun compileFile(options: String, srcPath: String, outDir: String, latch: CountDownLatch) {
    val sFile = SourceFile(srcPath)
    val dFile = DebugFile(outDir, srcPath, options)
    val faults = Faults()
    
    val lexer = Lexer(faults, sFile)
    if(runLexer(lexer, dFile)) return

    val parser = Parser(faults, lexer.tokens)
    if(runParser(parser, dFile)) return
    
    val publisher = Publisher(faults, parser.tree)
    if(runPublisher(publisher, dFile, latch)) return
    
    val oFile = OutputFile(outDir, srcPath)
    val transpiler = Transpiler(faults, publisher.tree, oFile)
    runTranspiler(transpiler, dFile)
}

fun runLexer(lexer: Lexer, dFile: DebugFile) = runStage(lexer.faults) { 
    println("Building ${lexer.file.path}...")
    lexer.makeTokens()
    dFile.write("Tokens:\n\t$lexer")
}

fun runParser(parser: Parser, dFile: DebugFile) = runStage(parser.faults) {
    parser.makeTree()
    dFile.write("AST:\n\t$parser")
}

fun runPublisher(publisher: Publisher, dFile: DebugFile, latch: CountDownLatch) 
    = runStage(publisher.faults) {
    publisher.publishTree()
    latch.countDown()
    latch.await()
    dFile.write("Published AST:\n\t$publisher")
}

fun runTranspiler(transpiler: Transpiler, dFile: DebugFile) = runStage(transpiler.faults) {
    transpiler.transpileTree()
    dFile.write("Transpiled")
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
