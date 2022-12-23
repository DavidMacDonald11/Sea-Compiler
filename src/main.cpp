#include "lexer/lexer.h"
#include "util/fault.h"

static void compileFile(const str& options, SourceFile& file);

int main(int argc, char *argv[]) {
    vector<str> args;
    for(int i = 0; i < argc; i++) args.push_back(argv[i]);

    str outDir = args[2];
    vector<str> filePaths(args.begin() + 3, args.end());

    for(const str& filePath : filePaths) {
        SourceFile file(filePath);
        compileFile(args[1], file);
    }

    return 0;
}

void compileFile(const str& options, SourceFile& file) {
    Lexer* lexer = nullptr;

    try {
        lexer = new Lexer(file);
        lexer->makeTokens();
        Fault::check();
    } catch(const Fault::CompilerFailure&) {
        fmt::print(stderr, "{}\n", Fault::toString());
    }

    if(in('d', options)) {
        fmt::print("{}:\n", file.path);
        fmt::print("  Tokens:\n    [{}]\n", (lexer) ? lexer->toString() : "");
    }

    delete lexer;
}
