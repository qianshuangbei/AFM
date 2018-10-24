#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QMainWindow>

class DataManager
{
public:
    unsigned int xSize;
    unsigned int ySize;
    QString date;
    QString description;
    QString rate;
    QString lineDirection;
    QString captureDirection;
    QString driveAmp;
    QString scanSize;
    QString ratio;
    QString ampSetPoint;
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
