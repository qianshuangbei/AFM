#include <iostream>
#include "datamanager.h"
#include <QFile>
#include <QTextStream>

DataManager* DataManager::_instance = NULL;
DataManager::DataManager()
{   
}

DataManager * DataManager::Instance(){

    _instance = new DataManager();

    return _instance;
}

bool DataManager::LoadRowFile(const QString &fileName){
    if(_instance == NULL){
        std::cout<<"ERROR: instance!!!";
    }
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
            _instance->description = curline;
        }else if(curline.contains("Data offset",Qt::CaseInsensitive)){
            curline.remove(0,14);
            curline.trimmed();
            const int tempnum = curline.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            _instance->data_offset.push_back(tempnum);
        }else if(curline.contains("Data length",Qt::CaseInsensitive)){
            curline.remove(0,14);
            curline.trimmed();
            const int tempnum = curline.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            _instance->data_length.push_back(tempnum);
        }else if(curline.contains("Image Data",Qt::CaseInsensitive)){
            curline.remove(0,curline.indexOf("\"")+1);
            const QString temp = curline.left(curline.size()-1);
            _instance->type.push_back(temp);
        }else if(curline.contains("2:Z scale:",Qt::CaseInsensitive)){
           QString tmp =curline.mid( curline.indexOf(")")+1, 8);
           tmp.trimmed();
           _instance->data_zscale.push_back(tmp.toDouble());
        }else if(curline.contains("@Sens. ZsensSens",Qt::CaseInsensitive)){
           QString tmp =curline.mid( curline.indexOf("V")+1, 8);
           tmp.trimmed();
           _instance->data_sscale.push_back(tmp.toDouble());
        }
        else if(curline.contains("Date:",Qt::CaseInsensitive)){
            curline.remove(0,7);
            curline.trimmed();
            _instance->date = curline;
        }else if(curline.contains("\\Scan Rate:",Qt::CaseInsensitive)){
            curline.remove(0,12);
            curline.trimmed();
            _instance->rate = curline;
        }else if(curline.contains("\\Valid data len X:",Qt::CaseInsensitive)){
            curline.remove(0,18);
            curline.trimmed();
            if (curline.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            _instance->_XSIZE = curline.toInt();
        }
        else if(curline.contains("\\Valid data len Y:",Qt::CaseInsensitive)){
            curline.remove(0,18);
            curline.trimmed();
            if (curline.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            _instance->_YSIZE = curline.toInt();
        }
        else if(curline.contains("\\Line Direction:",Qt::CaseInsensitive)){
            curline.remove(0,17);
            curline.trimmed();
            _instance->linedirection = curline;
        }
        else if(curline.contains("\\Capture direction:",Qt::CaseInsensitive)){
            curline.remove(0,20);
            curline.trimmed();
            _instance->capturedirection = curline;
        }
        else if(curline.contains("\\@2:CantDrive: ",Qt::CaseInsensitive)){
            curline.remove(0, curline.indexOf(")")+1);
            curline.trimmed();
            _instance->driveamp = curline;
        }
        else if(curline.contains("\\Scan Size:",Qt::CaseInsensitive)){
            curline.remove(0,12);
            curline.trimmed();
            int i=0;
            for(; i<curline.length();i++) if(curline[i]<'0'||curline[0]>'9') break;
            QString temp = curline.left(i);
            _instance->scansize = temp;
        }
        else if(curline.contains("\\@2:SCMFeedbackSetpoint:",Qt::CaseInsensitive)){
            curline.remove(0, curline.indexOf(")")+1);
            curline.trimmed();
            _instance->ampsetpoint = curline;
        }
        else if(curline.contains("\\Aspect Ratio:",Qt::CaseInsensitive)){
            curline.remove(0, 15);
            curline.trimmed();
            _instance->ratio = curline;
        }


    }
    while(!curline.contains("*File list end"));
    return false;
}

