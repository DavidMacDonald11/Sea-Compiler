package sea

import kotlinx.coroutines.*
import java.util.concurrent.CountDownLatch
import sea.lexer.SourceFile
import sea.lexer.SourceLine
import sea.lexer.Locale

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
    
    // publish

    latch.countDown()
    latch.await()

    // transpile
}
