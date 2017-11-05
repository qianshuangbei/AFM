#ifndef NANOMODULE_H
#define NANOMODULE_H
#include <QString>

struct Nano{
    // row Info;
    struct textinfo{
        QString date;
    };

    QString type;
    // data Info;
    int16_t **matrix;
};

class Nanodata
{
public:
    Nanodata();
    bool has_date();
    void set_date(const QString date);
    QString date();
private:
    Nano _nano;
};



#endif // NANOMODULE_H

