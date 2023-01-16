#include <filesystem>
#include <fmt/core.h>
#include "lexer/lexer.h"
#include "publisher/publisher.h"
#include "substitutor/substitutor.h"
#include "parser/parser.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "transpiler/output-file.h"
#include "fault.h"

static void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile);
static void debugOut(str options, str path, Lexer* lexer, Substitutor* sub, Parser* parser);


int main(int argc, char *argv[]) {
    vector<str> args;
    for(int i = 0; i < argc; i++) args.push_back(argv[i]);

    str outDir = args[2];
    vector<str> filePaths(args.begin() + 3, args.end());

    for(const str& filePath : filePaths) {
        SourceFile sFile(filePath);

        str outPath = replaceStr(filePath, ".sea", ".c");
        outPath = std::filesystem::path(outPath).filename();
        outPath = fmt::format("{}/{}", outDir, outPath);
        
        OutputFile oFile(outPath);
        compileFile(args[1], sFile, oFile);
    }

    return 0;
}

void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile) {
    Fault fault;
    Lexer* lexer = nullptr;
    Substitutor* substitutor = nullptr;
    Parser* parser = nullptr;
    Publisher* publisher = nullptr;
    Transpiler* transpiler = nullptr;

    try {
        fmt::print("Building {}...\n", sFile.path);
        lexer = new Lexer(fault, sFile);
        lexer->makeTokens();
        fault.check(); 

        substitutor = new Substitutor(fault, lexer->tokens);
        substitutor->replaceTokens();
        fault.check();

        parser = new Parser(fault, substitutor->tokens);
        parser->makeTree();
        fault.check();

        publisher = new Publisher(fault);
        fault.check();

        transpiler = new Transpiler(fault, oFile);
        parser->tree->transpile(*transpiler);
        fault.check();

        debugOut(options, sFile.path, lexer, substitutor, parser);
    } catch(const Fault::CompilerFailure&) {
        debugOut(options, sFile.path, lexer, substitutor, parser);
        fmt::print(stderr, "{}\n", fault.toString());
    }

    delete lexer;
    delete substitutor;
    delete parser;
    delete publisher;
    delete transpiler;
}

void debugOut(str options, str path, Lexer* lexer, Substitutor* sub, Parser* parser) {
    if(not in('d', options)) return;

    fmt::print("{}:\n", path);
    fmt::print("  Pre-Tokens:\n    [{}]\n", lexer? lexer->toString() : "");
    fmt::print("  Tokens:\n    [{}]\n", sub? sub->toString() : "");
    fmt::print("  AST:\n    {}\n", parser? parser->toString() : "");
    fmt::print("\n");
}
