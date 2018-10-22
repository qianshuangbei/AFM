#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QMainWindow>

class DataManager
{
public:
    unsigned int _XSIZE;
    unsigned int _YSIZE;
    QString date;
    QString description;
    QString rate;
    QString linedirection;
    QString capturedirection;
    QString driveamp;
    QString scansize;
    QString ratio;
    QString ampsetpoint;
    QVector<QString> type;
    QVector<uint32_t> data_offset;
    QVector<uint32_t> data_length;
    QVector<double> data_zscale;
    QVector<double> data_sscale;
    double **N_matrix;

public:
    bool LoadRowFile(const QString &filename);
    static DataManager *Instance();

protected:
    DataManager();

private:
    static DataManager *_instance;

};

#endif // DATAMANAGER_H
