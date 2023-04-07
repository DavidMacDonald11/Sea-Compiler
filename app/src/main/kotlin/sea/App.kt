package sea

import sea.lexer.SourceFile
import sea.lexer.SourceLine
import sea.lexer.Locale

class App {}

var threadsConstructedLock = Object()
var threadsConstructed = false
var threadsCompletedPublishing = 0

fun main(args: Array<String>) {
    val options = args[0]
    val outDir = args[1]
    val filePaths = args.drop(2)

    val threads = ArrayList<Thread>(filePaths.size)

    for((id, filePath) in filePaths.withIndex()) {
        val thread = Thread { compileFile(options, filePath, outDir, id) }
        threads.add(thread)
        thread.start()
    }

    synchronized(threadsConstructedLock) { threadsConstructed = true }
    for(thread in threads) thread.join()
}

fun compileFile(options: String, srcPath: String, outDir: String, id: Int) {
    val sourceFile = SourceFile(srcPath)
    
    synchronized(threadsConstructedLock) { 
        while(!threadsConstructed) Thread.sleep(100)
    }
}
