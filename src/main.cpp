#include <filesystem>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/node.h"
#include "util/fault.h"
#include "util/output-file.h"

namespace fs = std::filesystem;

static void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile);

int main(int argc, char *argv[]) {
    vector<str> args;
    for(int i = 0; i < argc; i++) args.push_back(argv[i]);

    str outDir = args[2];
    vector<str> filePaths(args.begin() + 3, args.end());

    for(const str& filePath : filePaths) {
        SourceFile sFile(filePath);

        str outPath = replaceStr(filePath, ".sea", ".c");
        outPath = fs::path(outPath).filename();
        outPath = fmt::format("{}/{}", outDir, outPath);
        
        OutputFile oFile(outPath);
        compileFile(args[1], sFile, oFile);
    }

    return 0;
}

void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile) {
    Lexer* lexer = nullptr;
    Parser* parser = nullptr;

    try {
        fmt::print("Building {}...\n", sFile.path);
        lexer = new Lexer(sFile);
        lexer->makeTokens();
        Fault::check();

        Node::parser = parser = new Parser(lexer->tokens);
        parser->makeTree();
        Fault::check();

        oFile.write(fmt::format("/**\n{}\n**/\n", parser->tree->toString()));
    } catch(const Fault::CompilerFailure&) {
        fmt::print(stderr, "{}\n", Fault::toString());
    }

    if(in('d', options)) {
        fmt::print("{}:\n", sFile.path);
        fmt::print("  Tokens:\n    [{}]\n", (lexer) ? lexer->toString() : "");
        fmt::print("  AST:\n    {}\n", (parser and parser->tree) ? parser->tree->toString() : "");
    }

    delete lexer;
    delete parser;
}
