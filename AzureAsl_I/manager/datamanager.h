#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QMainWindow>

class DataManager
{
public:
    unsigned int xSIZE;
    unsigned int ySIZE;
    QString date;
    QString description;
    QString rate;
    QString lineDirection;
    QString captureDirection;
    QString driveamp;
    QString scansize;
    QString ratio;
    QString ampsetpoint;
    QVector<QString> type;
    QVector<uint32_t> dataOffset;
    QVector<uint32_t> dataLength;
    QVector<double> dataZScale;
    QVector<double> dataSScale;
    double **matrix;

public:
    bool LoadRowFile(const QString &fileName);
    static DataManager *Instance();

protected:
    DataManager();

private:
    static DataManager *_instance;

};

#endif // DATAMANAGER_H
