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
    QString curLine;

    if(!file.open(QFile::ReadOnly|QFile::Text)){
        std::cout<< "ERROR: read file fail";
        return false;
    }
    QTextStream out(&file);
    curLine = out.readLine();
    if (!curLine.contains("\*File list")) {
        std::cout<< "ERROR: AFM Head error";
        return false;
    }
    do{
        curLine = out.readLine();
        if(curLine.contains("Description",Qt::CaseInsensitive)){
            curLine.remove(0,14);
            _instance->description = curLine;
        }else if(curLine.contains("Data offset",Qt::CaseInsensitive)){
            curLine.remove(0,14);
            curLine.trimmed();
            const int tempnum = curLine.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            _instance->dataOffset.push_back(tempnum);
        }else if(curLine.contains("Data length",Qt::CaseInsensitive)){
            curLine.remove(0,14);
            curLine.trimmed();
            const int tempnum = curLine.toInt();
            if(tempnum < 0){
                std::cout<< "ERROR: dataoffset is negative";
                return false;
            }
            _instance->dataLength.push_back(tempnum);
        }else if(curLine.contains("Image Data",Qt::CaseInsensitive)){
            curLine.remove(0,curLine.indexOf("\"")+1);
            const QString temp = curLine.left(curLine.size()-1);
            _instance->type.push_back(temp);
        }else if(curLine.contains("2:Z scale:",Qt::CaseInsensitive)){
           QString tmp =curLine.mid( curLine.indexOf(")")+1, 8);
           tmp.trimmed();
           _instance->dataZScale.push_back(tmp.toDouble());
        }else if(curLine.contains("@Sens. ZsensSens",Qt::CaseInsensitive)){
           QString tmp =curLine.mid( curLine.indexOf("V")+1, 8);
           tmp.trimmed();
           _instance->dataSScale.push_back(tmp.toDouble());
        }
        else if(curLine.contains("Date:",Qt::CaseInsensitive)){
            curLine.remove(0,7);
            curLine.trimmed();
            _instance->date = curLine;
        }else if(curLine.contains("\\Scan Rate:",Qt::CaseInsensitive)){
            curLine.remove(0,12);
            curLine.trimmed();
            _instance->rate = curLine;
        }else if(curLine.contains("\\Valid data len X:",Qt::CaseInsensitive)){
            curLine.remove(0,18);
            curLine.trimmed();
            if (curLine.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            _instance->xSize = curLine.toInt();
        }
        else if(curLine.contains("\\Valid data len Y:",Qt::CaseInsensitive)){
            curLine.remove(0,18);
            curLine.trimmed();
            if (curLine.toInt()<0) {
                std::cout<< "ERROR: image size is negative";
                return false;
            }
            _instance->ySize = curLine.toInt();
        }
        else if(curLine.contains("\\Line Direction:",Qt::CaseInsensitive)){
            curLine.remove(0,17);
            curLine.trimmed();
            _instance->lineDirection = curLine;
        }
        else if(curLine.contains("\\Capture direction:",Qt::CaseInsensitive)){
            curLine.remove(0,20);
            curLine.trimmed();
            _instance->captureDirection = curLine;
        }
        else if(curLine.contains("\\@2:CantDrive: ",Qt::CaseInsensitive)){
            curLine.remove(0, curLine.indexOf(")")+1);
            curLine.trimmed();
            _instance->driveAmp = curLine;
        }
        else if(curLine.contains("\\Scan Size:",Qt::CaseInsensitive)){
            curLine.remove(0,12);
            curLine.trimmed();
            int i=0;
            for(; i<curLine.length();i++) if(curLine[i]<'0'||curLine[0]>'9') break;
            QString temp = curLine.left(i);
            _instance->scanSize = temp;
        }
        else if(curLine.contains("\\@2:SCMFeedbackSetpoint:",Qt::CaseInsensitive)){
            curLine.remove(0, curLine.indexOf(")")+1);
            curLine.trimmed();
            _instance->ampSetPoint = curLine;
        }
        else if(curLine.contains("\\Aspect Ratio:",Qt::CaseInsensitive)){
            curLine.remove(0, 15);
            curLine.trimmed();
            _instance->ratio = curLine;
        }


    }
    while(!curLine.contains("*File list end"));
    return true;
}

