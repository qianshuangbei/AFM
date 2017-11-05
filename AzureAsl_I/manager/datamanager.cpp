#include "datamanager.h"

DataManager::DataManager()
{

}

bool DataManager::LoadRowFile(const QString &filename, Nanodata *response){
    QFile file(fileName);
    QString curline;

    if(!file.open(QFile::ReadOnly|QFile::Text)){
        cout<< "ERROR: read file fail";
        return false;
    }
    QTextStream out(&file);
    curline = out.readLine();
    if (!curline.contains("File list")) {
        cout<< "ERROR: AFM Head error";
        return false;/
    }
    do{
    curline = out.readLine();

    if(curline.contains("Description",Qt::CaseInsensitive))
    {
        curline.remove(0,14);
        r_description = curline;

    }
    else if(curline.contains("Data offset",Qt::CaseInsensitive))
    {
        curline.remove(0,14);
        curline.trimmed();
        r_offset.push_back(curline.toFloat());
    }
    else if(curline.contains("Data length",Qt::CaseInsensitive))
    {
        curline.remove(0,14);
        curline.trimmed();
        r_length.push_back(curline.toFloat());
    }
    else if(curline.contains("Image Data",Qt::CaseInsensitive)){
        curline.remove(0,curline.indexOf("\"")+1);
        QString tmp = curline.left(curline.size()-1);
        r_type.push_back(tmp);

    }
    else if(curline.contains("2:Z scale:",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf(")")+1, 8);
       tmp.trimmed();
       r_zscale.push_back(tmp.toFloat());

    }
    else if(curline.contains("@Sens. ZsensSens",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("V")+1, 8);
       tmp.trimmed();
       r_sscale=  tmp.toFloat();
    }
    else if(curline.contains("Date:",Qt::CaseInsensitive)){
        curline.remove(0,7);
        curline.trimmed();
        r_date=curline;
    }
    else if(curline.contains("\\Scan Rate:",Qt::CaseInsensitive)){
        curline.remove(0,12);
        curline.trimmed();
        r_rate=curline;
    }
    else if(curline.contains("\\Valid data len X:",Qt::CaseInsensitive)){
        curline.remove(0,18);
        curline.trimmed();
        r_samlpel=curline;
    }
    else if(curline.contains("\\Valid data len Y:",Qt::CaseInsensitive)){
        curline.remove(0,18);
        curline.trimmed();
        r_line=curline;
    }
    else if(curline.contains("\\Line Direction:",Qt::CaseInsensitive)){
        curline.remove(0,17);
        curline.trimmed();
        r_LD=curline;
    }
    else if(curline.contains("\\Capture direction:",Qt::CaseInsensitive)){
        curline.remove(0,20);
        curline.trimmed();
        r_CD=curline;
    }
    else if(curline.contains("\\@2:CantDrive: ",Qt::CaseInsensitive)){
        curline.remove(0, curline.indexOf(")")+1);
        curline.trimmed();
        r_DA=curline;
    }
    else if(curline.contains("\\Scan Size:",Qt::CaseInsensitive)){
        curline.remove(0,12);
        curline.trimmed();
        int i=0;
        for(; i<curline.length();i++) if(curline[i]<'0'||curline[0]>'9') break;
        QString temp = curline.left(i);
        r_SS.push_back(temp);
    }
    else if(curline.contains("\\@2:SCMFeedbackSetpoint:",Qt::CaseInsensitive)){
        curline.remove(0, curline.indexOf(")")+1);
        curline.trimmed();
        r_AS=curline;
    }
    else if(curline.contains("\\Aspect Ratio:",Qt::CaseInsensitive)){
        curline.remove(0, 15);
        curline.trimmed();
        r_AR=curline;
    }

    }
    while(!curline.contains("*File list end"));
    return false;
}

bool DataManager::LoadDataFile(const QString &filename, Nanodata *response){

}
