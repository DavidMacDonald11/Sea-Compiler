#include <filesystem>
#include "publisher/publisher-file.h"


PublisherFile::PublisherFile(str outDir, str srcPath)
: file(nullptr), outDir(outDir), srcPath(srcPath) {
    path = srcPath + "lib";
    path = std::filesystem::path(path).filename();
    path = fmt::format("{}/{}", outDir, path);
}

PublisherFile::~PublisherFile() {
    if(file) file->close();
    delete file;
}

void PublisherFile::write(const str &s, str end) {
    if(not file) file = new std::ofstream(path);
    *file << s << end;
}
