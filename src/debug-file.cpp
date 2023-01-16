#include <filesystem>
#include "debug-file.h"

DebugFile::DebugFile(str outDir, str srcPath, str options)
: file(nullptr), outDir(outDir), srcPath(srcPath) {
    path = srcPath + "debug";
    path = std::filesystem::path(path).filename();
    path = fmt::format("{}/{}", outDir, path);

    if(in('d', options)) {
        file = new std::ofstream(path);
        write(fmt::format("{}:", srcPath));
    }
}

DebugFile::~DebugFile() {
    if(file) file->close();
    delete file;
}

void DebugFile::write(const str &s, str end) {
    if(file) *file << s << end;
}
