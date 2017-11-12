#ifndef NANOMODULE_H
#define NANOMODULE_H
#include <QString>
#include <QVector>

class Nanodata
{
public:
    struct Nano{

        QString date;
        QString description;
        QString type;
        QString rate;
        QString Xsize;
        QString Ysize;
        QString linedirection;
        QString capturedirection;
        QString driveamp;
        QString scansize;
        QString ratio;
        QString ampsetpoint;
        QVector<uint32_t> data_offset;
        QVector<uint32_t> data_length;
        QVector<double> data_zscale;
        QVector<double> data_sscale;

        // data Info;
        int16_t **matrix;
    };

    Nanodata();
    bool has_date();
    bool has_description();
    bool has_type();
    bool has_rate();
    bool has_Xsize();
    bool has_Ysize();
    bool has_linedirection();
    bool has_capturedirection();
    bool has_driveamp();
    bool has_scansize();
    bool has_ratio();
    bool has_ampsetpoint();
    uint32_t has_data_offset();
    uint32_t has_data_length();
    uint32_t has_data_zscale();
    uint32_t has_data_sscale();

    void set_date(const QString &input);
    void set_description(const QString &input);
    void set_type(const QString &input);
    void set_rate(const QString &input);
    void set_Xsize(const QString &input);
    void set_Ysize(const QString &input);
    void set_linedirection(const QString &input);
    void set_capturedirection(const QString &input);
    void set_driveamp(const QString &input);
    void set_scansize(const QString &input);
    void set_ratio(const QString &input);
    void set_ampsetpoint(const QString &input);
    void append_data_offset(const int &input);
    void append_data_length(const int &input);
    void append_data_zscale(const double &input);
    void append_data_sscale(const double &input);

    QString date();
    QString description();
    QString type();
    QString rate();
    QString Xsize();
    QString Ysize();
    QString linedirection();
    QString capturedirection();
    QString driveamp();
    QString scansize();
    QString ratio();
    QString ampsetpoint();
    int data_offset(const uint32_t &input);
    int data_length(const uint32_t &input);
    double data_zscale(const uint32_t &input);
    double data_sscale(const uint32_t &input);

private:
    Nano _nano;
};



#endif // NANOMODULE_H

