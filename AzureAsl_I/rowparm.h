#ifndef ROWPARM_H
#define ROWPARM_H
#include <QMainWindow>

class RowParm
{

public:
//  RowParm();
     QString r_date;
     QString r_rate;
     QString r_samlpel;
     QString r_line;
     QString r_LD;    // line direction
     QString r_CD;    //capture Direction
     QString r_AS;    //amplitude setpoint
     QString r_DA;    //drive amplitude
     QString r_AR;    //aspect ratio
     QString r_description;

     float r_sscale;
    QVector<float> r_offset;
    QVector<float> r_length;
    QVector<float> r_zscale;
    QVector<QString> r_type;
    QVector<QString> r_SS;  //Scan Size

    bool loadinfo(const QString &fileName);
    QString extrainfo(const QString &fileName , const QString name);
    bool loaddatFile (const QString &fileName, const int Ti);

private:

};

#endif // ROWPARM_H
