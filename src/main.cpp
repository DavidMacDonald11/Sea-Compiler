#include <filesystem>
#include "lexer/lexer.h"
#include "util/fault.h"
#include "util/output_file.h"

namespace fs = std::filesystem;

static void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile);

int main(int argc, char *argv[]) {
    vector<str> args;
    for(int i = 0; i < argc; i++) args.push_back(argv[i]);

    str outDir = args[2];
    vector<str> filePaths(args.begin() + 3, args.end());

    for(const str& filePath : filePaths) {
        SourceFile sFile(filePath);

        str outPath = replaceStr(filePath, ".hyd", ".ll");
        outPath = fs::path(outPath).filename();
        outPath = fmt::format("{}/{}", outDir, outPath);
        
        OutputFile oFile(outPath);
        compileFile(args[1], sFile, oFile);
    }

    return 0;
}

void compileFile(const str& options, SourceFile& sFile, OutputFile& oFile) {
    Lexer* lexer = nullptr;

    try {
        fmt::print("Building {}...\n", sFile.path);
        lexer = new Lexer(sFile);
        lexer->makeTokens();
        Fault::check();

        oFile.write(lexer->toString());
    } catch(const Fault::CompilerFailure&) {
        fmt::print(stderr, "{}\n", Fault::toString());
    }

    if(in('d', options)) {
        fmt::print("{}:\n", sFile.path);
        fmt::print("  Tokens:\n    [{}]\n", (lexer) ? lexer->toString() : "");
    }

    delete lexer;
}
