#include <filesystem>
#include "transpiler/output-file.h"

OutputFile::OutputFile(str outDir, str srcPath) {
    path = replaceStr(srcPath, ".sea", ".c");
    path = std::filesystem::path(path).filename();
    path = fmt::format("{}/{}", outDir, path);
    
    file = new std::ofstream(path);
}

OutputFile::~OutputFile() {
    file->close();
    delete file;
}

void OutputFile::write(const str& text) {
    *file << text;
}