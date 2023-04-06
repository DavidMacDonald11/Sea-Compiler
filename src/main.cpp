#include <atomic>
#include <chrono>
#include <fmt/core.h>
#include <thread>
#include "lexer/lexer.h"
#include "lexer/source-file.h"
#include "publisher/publisher.h"
#include "publisher/publisher-file.h"
#include "substitutor/substitutor.h"
#include "parser/parser.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "transpiler/output-file.h"
#include "fault.h"
#include "debug-file.h"

using namespace std::chrono_literals;
using std::atomic;
using std::thread;

static void compileFile(str options, str srcPath, str outDir, nat id);
static Lexer* runLexer(Fault& fault, SourceFile& sFile, DebugFile& dFile);
static Substitutor* runSubstitutor(Lexer& lexer, DebugFile& dFile);
static Parser* runParser(Substitutor& substitutor, DebugFile& dFile);
static Publisher* runPublisher(Parser& parser, PublisherFile& pFile, DebugFile& dFile);
static Transpiler* runTranspiler(Parser& parser, OutputFile& oFile, DebugFile& dFile);

static atomic<bool> threadsConstructed = false;
static atomic<nat> threadsCompletedPublishing = 0;
static atomic<nat> threadsCompletedTranspiling = 0;
static atomic<nat> threadCount = 0;
static atomic<Publisher*>* publishers;
#define sleepTime 100ms

int main(int argc, char *argv[]) {
    vector<str> args;
    for(int i = 0; i < argc; i++) args.push_back(argv[i]);

    str options = args[1];
    str outDir = args[2];

    vector<str> filePaths(args.begin() + 3, args.end());
    vector<thread> threads;
    
    threads.reserve(threadCount = filePaths.size());
    publishers = new atomic<Publisher*>[threadCount];
    nat id = 0;

    for(const str& filePath : filePaths) {
        threads.push_back(thread(compileFile, options, filePath, outDir, id++));
    }

    threadsConstructed = true;
    for(thread& t : threads) { if(t.joinable()) t.join(); }

    for(nat i = 0; i < threadCount; i++) delete publishers[i];
    delete[] publishers;
    
    return 0;
}

void compileFile(str options, str srcPath, str outDir, nat id) {
    Fault fault;
    SourceFile sFile(srcPath);
    PublisherFile pFile(outDir, srcPath);
    DebugFile dFile(outDir, srcPath, options);
    
    Lexer* lexer = runLexer(fault, sFile, dFile);
    Substitutor* substitutor = (lexer)? runSubstitutor(*lexer, dFile) : nullptr;
    Parser* parser = (substitutor)? runParser(*substitutor, dFile) : nullptr;
    Publisher* publisher = (parser)? runPublisher(*parser, pFile, dFile) : nullptr;

    while(not threadsConstructed) std::this_thread::sleep_for(sleepTime);
    threadsCompletedPublishing += 1;
    publishers[id] = publisher;

    if(not publisher) {
        delete lexer;
        delete substitutor;
        delete parser;
        return;
    }

    OutputFile oFile(outDir, srcPath);
    while(threadsCompletedPublishing != threadCount) std::this_thread::sleep_for(sleepTime);
    Transpiler* transpiler = runTranspiler(*parser, oFile, dFile);
    threadsCompletedTranspiling += 1;

    while(threadsCompletedTranspiling != threadCount) std::this_thread::sleep_for(sleepTime);

    delete lexer;
    delete substitutor;
    delete parser;
    delete transpiler;
}

Lexer* runLexer(Fault& fault, SourceFile& sFile, DebugFile& dFile) {
    bool failed = false;
    Lexer* lexer = nullptr;

    try {
        fmt::print("Building {}...\n", sFile.path);
        lexer = new Lexer(fault, sFile);
        lexer->makeTokens();
        fault.check(); 
        fault.stage++;
    } catch(const Fault::CompilerFailure&) {
        failed = true;
        fmt::print(stderr, "{}\n", fault.toString());
    }

    dFile.write(fmt::format("Pre-Tokens:\n\t[{}]", lexer? lexer->toString() : ""));
    return failed? nullptr: lexer;
}

Substitutor* runSubstitutor(Lexer& lexer, DebugFile& dFile) {
    bool failed = false;
    Substitutor* substitutor = nullptr;

    try {
        substitutor = new Substitutor(lexer.fault, lexer.tokens);
        substitutor->replaceTokens();
        substitutor->fault.check();
        substitutor->fault.stage++;
    } catch(const Fault::CompilerFailure&) { 
        failed = true;
        fmt::print(stderr, "{}\n", substitutor->fault.toString()); 
    }

    dFile.write(fmt::format("Tokens:\n\t[{}]", substitutor? substitutor->toString() : ""));
    return failed? nullptr : substitutor;
}

Parser* runParser(Substitutor& substitutor, DebugFile& dFile) {
    bool failed = false;
    Parser* parser = nullptr;

    try {
        parser = new Parser(substitutor.fault, substitutor.tokens);
        parser->makeTree();
        parser->fault.check();
        parser->fault.stage++;
    } catch(const Fault::CompilerFailure&) { 
        failed = true;
        fmt::print(stderr, "{}\n", parser->fault.toString());
    }

    dFile.write(fmt::format("AST:\n\t{}", parser? parser->toString() : ""));
    return failed? nullptr : parser;
}

Publisher* runPublisher(Parser& parser, PublisherFile& pFile, DebugFile& dFile) {
    bool failed = false;
    Publisher* publisher = nullptr;

    try {
        publisher = new Publisher(parser.fault, pFile);
        parser.tree->publish(*publisher);
        publisher->fault.check();
        publisher->fault.stage++;
    } catch(const Fault::CompilerFailure&) { 
        failed = true;
        fmt::print(stderr, "{}\n", publisher->fault.toString()); 
    }

    dFile.write(fmt::format("Published:\n{}", publisher? publisher->toString() : ""));
    return failed? nullptr : publisher;
}

Transpiler* runTranspiler(Parser& parser, OutputFile& oFile, DebugFile&) {
    bool failed = false;
    Transpiler* transpiler = nullptr;

    try {
        transpiler = new Transpiler(parser.fault, oFile);
        parser.tree->transpile(*transpiler);
        transpiler->fault.check();
        transpiler->fault.stage++;
    } catch(const Fault::CompilerFailure&) {
        failed = true;
        fmt::print(stderr, "{}\n", transpiler->fault.toString());
    }

    return failed? nullptr : transpiler;
}
