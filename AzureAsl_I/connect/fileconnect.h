#ifndef FILECONNECT_H
#define FILECONNECT_H
#include "filemodule.h"
#include "manager/nanomodule.h"

class fileconnect
{
public:
    fileconnect();
    bool ReadFile(const QString &filename);
    bool MemoryFile();
private:
//    FileInfo _fileinfo;
};

#endif // FILECONNECT_H
