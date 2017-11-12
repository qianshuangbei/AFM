#include <iostream>
#include "datamanager.h"
#include <QFile>
#include <QTextStream>


DataManager::DataManager()
{

}

bool DataManager::LoadRowFile(const QString &fileName, Nanodata *response){
    QFile file(fileName);
    QString curline;

    if(!file.open(QFile::ReadOnly|QFile::Text)){
        std::cout<< "ERROR: read file fail";
        return false;
    }
    QTextStream out(&file);
    curline = out.readLine();
    if (!curline.contains("File list")) {
        std::cout<< "ERROR: AFM Head error";
        return false;
    }
    do{
        curline = out.readLine();

        if(curline.contains("Description",Qt::CaseInsensitive)){
            curline.remove(0,14);
            response->set_date(curline);
        }else if(curline.contains("Data offset",Qt::CaseInsensitive)){
            curline.remove(0,14);
            curline.trimmed();
            const int tempnum = curline.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            response->append_data_offset(tempnum);
        }else if(curline.contains("Data length",Qt::CaseInsensitive)){
            curline.remove(0,14);
            curline.trimmed();
            const int tempnum = curline.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            response->append_data_length(tempnum);
        }else if(curline.contains("Image Data",Qt::CaseInsensitive)){
            curline.remove(0,curline.indexOf("\"")+1);
            const QString temp = curline.left(curline.size()-1);
            response->set_type(temp);
        }else if(curline.contains("2:Z scale:",Qt::CaseInsensitive)){
           QString tmp =curline.mid( curline.indexOf(")")+1, 8);
           tmp.trimmed();
           response->append_data_zscale(tmp.toDouble());
        }else if(curline.contains("@Sens. ZsensSens",Qt::CaseInsensitive)){
           QString tmp =curline.mid( curline.indexOf("V")+1, 8);
           tmp.trimmed();
           response->append_data_sscale(tmp.toDouble());
        }
        else if(curline.contains("Date:",Qt::CaseInsensitive)){
            curline.remove(0,7);
            curline.trimmed();
            response->set_date(curline);
        }else if(curline.contains("\\Scan Rate:",Qt::CaseInsensitive)){
            curline.remove(0,12);
            curline.trimmed();
            response->set_rate(curline);
        }else if(curline.contains("\\Valid data len X:",Qt::CaseInsensitive)){
            curline.remove(0,18);
            curline.trimmed();
            if (curline.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            response->set_Xsize(curline);
        }
        else if(curline.contains("\\Valid data len Y:",Qt::CaseInsensitive)){
            curline.remove(0,18);
            curline.trimmed();
            if (curline.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            response->set_Ysize(curline);
        }
        else if(curline.contains("\\Line Direction:",Qt::CaseInsensitive)){
            curline.remove(0,17);
            curline.trimmed();
            response->set_linedirection(curline);
        }
        else if(curline.contains("\\Capture direction:",Qt::CaseInsensitive)){
            curline.remove(0,20);
            curline.trimmed();
            response->set_capturedirection(curline);
        }
        else if(curline.contains("\\@2:CantDrive: ",Qt::CaseInsensitive)){
            curline.remove(0, curline.indexOf(")")+1);
            curline.trimmed();
            response->set_driveamp(curline);
        }
        else if(curline.contains("\\Scan Size:",Qt::CaseInsensitive)){
            curline.remove(0,12);
            curline.trimmed();
            int i=0;
            for(; i<curline.length();i++) if(curline[i]<'0'||curline[0]>'9') break;
            QString temp = curline.left(i);
            response->set_scansize(temp);
        }
        else if(curline.contains("\\@2:SCMFeedbackSetpoint:",Qt::CaseInsensitive)){
            curline.remove(0, curline.indexOf(")")+1);
            curline.trimmed();
            response->set_ampsetpoint(curline);
        }
        else if(curline.contains("\\Aspect Ratio:",Qt::CaseInsensitive)){
            curline.remove(0, 15);
            curline.trimmed();
            response->set_ratio(curline);
        }


    }
    while(!curline.contains("*File list end"));
    return false;
}

bool DataManager::LoadDataFile(const QString &filename, Nanodata *response){

}
