#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "nanomodule.h"

class DataManager
{
public:
    bool LoadDataFile(const QString &filename, Nanodata *response);
    bool LoadRowFile(const QString &filename, Nanodata *response);
    DataManager();
};

#endif // DATAMANAGER_H
