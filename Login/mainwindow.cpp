#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myfunction.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include<QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    curFile =tr("AZure");
    setWindowTitle(curFile);
    screeningflag =true;
    bgflag = true;
    ui->Address->setText("E:\\Graduate List\\Rabbit IGg\\Biology_first\\20161130");
    ui->FrontFileName->setText("20161130.001");
    ui->BackFileName->setText("20161130.098");
    ui->UpBox->setValue(80);
    ui->DownBox->setValue(200);
    ui->ThresholdBox->setValue(0.2);
    ui->ScaleDown->setValue(0);
    ui->Scaleup->setValue(600);
    ui->YScale->setValue(10);



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_Address_selectionChanged()
{
    QString T_filename;
    T_filename= QFileDialog::getExistingDirectory(this);
    ui->Address->setText(T_filename);
}



void MainWindow::on_LoadButton_clicked()
{

    QString fileaddress,frontfile, backfile, cur;
    T_filelist.clear();
    fileaddress = ui->Address->text();
    frontfile = ui->FrontFileName->text();
    backfile = ui->BackFileName->text();
    int pos, numfront, numback, addzero;

    pos = frontfile.indexOf(".");
    QString T_frontfile=frontfile.right(frontfile.length()-pos-1);
    addzero = T_frontfile.length();
    frontfile = frontfile.left(pos+1);
    numfront = T_frontfile.toInt();

    pos = backfile.indexOf(".");
    T_frontfile=backfile.right(backfile.length()-pos-1);
    backfile = backfile.left(pos+1);
    numback = T_frontfile.toInt();

    if(numback<numfront) return ;
    if(frontfile != backfile) return ;

    QStringList filelist;
    for(int i =numfront; i<=numback; i++)
   {

    QString s = QString::number(i,10);
    while(s.length()<addzero) s= "0"+s;
    cur =fileaddress+"\\"+frontfile+s;

    QFile File(cur);
     if(File.exists()){ filelist.append(cur);T_filelist.push_back(cur);}
    else continue;
     File.close();
   }



    QStringListModel *model= new QStringListModel(filelist);
    ui->listView->setModel(model);

}



void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    ui->DMTedit->setText("0.00");
    QString cur= index.data(Qt::DisplayRole).toString();
    loadrowFile(cur);
    loaddatFile(cur);
    showcurve();
    curIndex = index;
    //DMT model ;

    double radiu= 10;
    double possion = 0.5;
    int length = T_datalength[1]/4;
    double curE = DMTmodel(N_matrix[1], radiu, possion , length, Z_size);
    curE = curE*1000000;

    QString s =QString::number(curE, 'g', 6)+"kPa";
    ui->DMTedit->setText(s);







}

void MainWindow::showcurve()
{
    QVector<double> Po_retrc,Po_trace;
    QVector<double> x,upx,downx,uptodown;
    QVector<double> polyfit, polyx;
    double T_min= N_matrix[0][0];
    double T_max= N_matrix[0][0];
    int len=T_datalength[1]/4;

    for(int i=0; i<len-10; i++)
    {
        Po_trace.push_back(N_matrix[0][i]);
        Po_retrc.push_back(N_matrix[1][i]);
        x.push_back(i*Z_size);

        T_max = N_matrix[0][i]>T_max?N_matrix[0][i]: T_max;\
        T_max = N_matrix[1][i]>T_max?N_matrix[1][i]: T_max;

        T_min = N_matrix[1][i]<T_min?N_matrix[1][i]: T_min;
        T_min = N_matrix[0][i]<T_min?N_matrix[0][i]: T_min;


    }
    upx.push_back(ui->UpBox->value());
    upx.push_back(ui->UpBox->value());
    downx.push_back(ui->DownBox->value());
    downx.push_back(ui->DownBox->value());


    int xlen=ui->DownBox->value()-ui->UpBox->value();
    int UpBoxval = ui->UpBox->value();
    int *x_matrix = new int [xlen];
    for(int i=0; i<xlen; i++)
    {
        x_matrix[i] = UpBoxval+i;
    }
    double curpoly[2];
    F_polyfit(N_matrix[1]+UpBoxval, x_matrix, xlen , curpoly);



    polyfit.push_back(curpoly[1]);
    polyx.push_back(0);
    polyx.push_back(ui->DownBox->value());
    polyfit.push_back( curpoly[0]*ui->DownBox->value()+curpoly[1]);






    double det= (T_max-T_min)*0.2;

    uptodown.push_back(T_min-det);
    uptodown.push_back(T_max+det);
    customPlot = new QCustomPlot(this);
    customPlot->setGeometry(440,70,830, 270);
    customPlot->addGraph();
    customPlot->graph(0)->setData(x,Po_retrc);
    customPlot->graph(0)->setPen(QPen(Qt::red));
    customPlot->addGraph();
    customPlot->graph(1)->setData(x,Po_trace);
    customPlot->addGraph();
    customPlot->graph(2)->setData(upx,uptodown);
    customPlot->graph(2)->setPen(QPen(Qt::green));
    customPlot->graph(2)->setPen(QPen(Qt::DashLine));
    customPlot->addGraph();
    customPlot->graph(3)->setData(downx,uptodown);
    customPlot->graph(3)->setPen(QPen(Qt::green));
    customPlot->graph(3)->setPen(QPen(Qt::DashLine));
    customPlot->addGraph();
    customPlot->graph(4)->setData(polyx, polyfit);
    customPlot->graph(4)->setPen(QPen(Qt::yellow));
    customPlot->graph(4)->setPen(QPen(Qt::DashLine));

   //设置坐标轴名称
    customPlot->xAxis->setLabel("Height Sensor(um)");
    customPlot->yAxis->setLabel("Force(nN)");

   //设置坐标轴显示范围，否则只能看到默认范围
    customPlot->xAxis->setRange(-20, len*Z_size+30);
    customPlot->yAxis->setRange(T_min-det, T_max+det);
    customPlot->replot();
    customPlot->show();

}





bool MainWindow::loadrowFile(const QString &fileName)
{

    QFile file(fileName);
    QString curline;
    //------------- parameter--------------//

    T_dataoffset.clear();
    T_datalength.clear();


    //------------- parameter--------------//
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this, tr("多文档编辑器"),tr("无法写入文件%1：/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    do{
    curline = out.readLine();


    if(curline.contains("Data offset",Qt::CaseInsensitive))
    {
        curline.remove(0,14);
        curline.trimmed();
        T_dataoffset.push_back(curline.toDouble());
    }
    else if(curline.contains("Data length",Qt::CaseInsensitive))
    {
        curline.remove(0,14);
        curline.trimmed();
        T_datalength.push_back( curline.toInt());
    }

    else if(curline.contains("Z scale: V [Sens. DeflSens]",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("(")+1, 10);
       tmp.trimmed();
       T_defsen= tmp.toDouble();
       tmp =curline.mid( curline.indexOf(")")+1, 6);
       tmp.trimmed();
       T_zscale=tmp.toDouble();
       T_zscale=12.81435;

    }

    else if(curline.contains("Spring Constant",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf(":")+1, 8);
       tmp.trimmed();
       T_k.push_back(tmp.toDouble());
   //    T_k.push_back(0.184900);

    }
    else if(curline.contains("@Sens. DeflSens:",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("V")+1, 6);
       tmp.trimmed();
       T_TMD= tmp.toDouble();

    }
    else if(curline.contains("@Sens. Zsens:",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("V")+1, 6);
       tmp.trimmed();
       T_zsen= tmp.toDouble();

    }



    }
    while(!curline.contains("*File list end"));

    file.close();

    return true;
}
bool MainWindow::loaddatFile(const QString &fileName)
{
    QFile file(fileName);
    char* a_pData = NULL;
    a_pData = (char*)malloc(T_datalength[1]);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::warning(this, tr("多文档编辑器"),tr("无法写入文件%1：/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    file.seek(T_dataoffset[0]);
    file.read(a_pData, T_datalength[1]);
    int len=T_datalength[1]/4;

    int m=0;
    for(int p=0 ;p<2; p++){
        for(int i =0; i<len; i++)
        {
            QString str1;
            QString str2;
            str1.sprintf("%2.2X",a_pData[2*m]);
            str2.sprintf("%2.2X",a_pData[2*m+1]);
            m++;
            if(str1.length()>2)
                str1=str1.right(2);
            if(str2.length()>2)
                str2=str2.right(2);
                str2+=str1;
            if(str2[0]>'7')
            {
                QString s1;
                s1=str2.right(3);
                bool ok;
                int dec = s1.toInt(&ok, 16);
                dec = dec-4096;
                N_matrix[p][i]= dec*T_defsen*T_TMD*T_k[0];

            }
           else{

                bool ok;
                int dec = str2.toInt(&ok, 16);
                N_matrix[p][i]= dec*T_defsen*T_TMD*T_k[0];

            }
        }

    }
    Z_size = T_zscale*T_zsen/(len-1);    //z向灵敏度
  //  Z_size =1.000;    //z向灵敏度
    file.close();
    return true;
}

void MainWindow::on_ScreenButton_clicked()
{
   goodlist.clear();
   badlist.clear();
   gooddata.clear();
   /*
   DMTdata.clear();

   //DMT model ;
   for(QVector<QString>:: iterator it=T_filelist.begin();it!=T_filelist.end(); ++it )
   {
       QString T_filename= *it;
       loadrowFile(T_filename);
       loaddatFile(T_filename);
       double radiu= 10;
       double possion = 0.5;
       int length = T_datalength[1]/4;
  //     double curE = DMTmodel(N_matrix[1], radiu, possion, length, Z_size );
       double curE = DMTmodel(N_matrix[1], radiu, possion , length, Z_size);
       curE = curE*1000000;
       DMTdata.append(curE);

   }
*/

   for(QVector<QString>:: iterator it=T_filelist.begin();it!=T_filelist.end(); ++it )
   {
       QString T_filename= *it;
       loadrowFile(T_filename);
       loaddatFile(T_filename);



       //----------No  peak--------------//
       int minpos;
       double curpoly[2];
       double distance;
       double P_min=N_matrix[0][0];
       for(int i=0; i<70; i++) {
           if(N_matrix[1][i]<P_min)
           {
               P_min = N_matrix[1][i];
               minpos= i ;
           }
       }
       int xlen=ui->DownBox->value()-ui->UpBox->value();
       int UpBoxval = ui->UpBox->value();
       int *x_matrix = new int [xlen];

       for(int i=0; i<xlen; i++)     x_matrix[i] = UpBoxval+i;
       F_polyfit(N_matrix[1]+UpBoxval, x_matrix, xlen , curpoly);
       distance = curpoly[0]*minpos+curpoly[1]-P_min;

       if(distance<ui->ThresholdBox->value()) continue;

       //-----trace & retrace 不重合-----//
       double re_avg=0, tr_avg=0, T_diff=0;
       for(int i =50; i<100;i++)
       {
           re_avg+= N_matrix[1][5*i];
           tr_avg+= N_matrix[0][5*i];

       }
       for(int i =440; i<450;i++) T_diff+=qAbs(N_matrix[0][i]-N_matrix[0][i-1]);
       for(int i =490; i<500;i++) T_diff+=qAbs(N_matrix[0][i]-N_matrix[0][i-1]);
       re_avg=re_avg/50;
       tr_avg=tr_avg/50;
       if(qAbs(re_avg-tr_avg)> T_diff/3) {
           badlist.append(*it);
           continue;
       }

        goodlist.append(*it);
        gooddata.append(distance);



  }


   //----------show list-------------//

   QStringListModel *model= new QStringListModel(goodlist);
   ui->listView->setModel(model);

}
/*
void MainWindow::on_good_clicked()
{
    bgflag =true;
    screeningflag =false;
}

void MainWindow::on_bad_clicked()
{
    bgflag =false;
    screeningflag =false;
}
*/
void MainWindow::on_DeleteButton_clicked()
{
    QString T_row=curIndex.data(Qt::DisplayRole).toString();
    int listIndex= goodlist.indexOf(T_row);
    goodlist.removeOne(T_row);    
    gooddata.removeAt(listIndex);
    QStringListModel *model= new QStringListModel(goodlist);
    ui->listView->setModel(model);




}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
// 不能用  因为Focus不在上面
    /*
    if(event->key()==Qt::Key_Down)
    {

        int row =curIndex.row();
        QModelIndex parentIndex = curIndex.parent();
        if((row-1)>=0&&(row-1)<goodlist.size())
        {
        curIndex = parentIndex.child(row-1,0);
        QString cur= curIndex.data(Qt::DisplayRole).toString();
        loadrowFile(cur);
        loaddatFile(cur);
        showcurve();
        }



    }
*/
    if ( (event->key() == Qt::Key_A))
    {        
        QString T_row=curIndex.data(Qt::DisplayRole).toString();
        int listIndex= goodlist.indexOf(T_row);
        goodlist.removeOne(T_row);
        gooddata.removeAt(listIndex);
        QStringListModel *model= new QStringListModel(goodlist);
        ui->listView->setModel(model);
    }


}

void MainWindow::on_StatisticButton_clicked()
{

    QMap<int, int> S_table;
    S_table.clear();
    for(int i =0; i<gooddata.size();i++)
    {
        int T=gooddata[i]/0.01;
        if(!S_table.count(T)) S_table[T]=1;
        else S_table[T]=S_table[T]+1;
    }
    QVector<double> datax;
    QVector<double> datay;
    datax.clear();
    datay.clear();

    for(QMap<int, int >:: const_iterator it=S_table.begin(); it!=S_table.end();++it)
    {
       datax.push_back(it.key()*10.00);
       datay.push_back(it.value());

    }
    S_cust= new QCustomPlot(this);
    S_cust->setGeometry(560,420,600,240);
    QCPBarsGroup *group = new QCPBarsGroup(S_cust);
    QCPBars *S_bar =new QCPBars(S_cust->xAxis, S_cust->yAxis);
    S_bar->setData(datax, datay);
    S_bar->setBrush(QColor(180,0,120));
    S_bar->setPen(QColor(0,0,0));
    S_bar->setWidth(10);
    S_bar->setBarsGroup(group);
    S_cust->xAxis->setLabel("Force(pN)");
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->yAxis->setRange(0,ui->YScale->value());
    S_cust->replot();
    S_cust->show();

}









void MainWindow::on_actionSelect_S_triggered()
{
    this->close();


}








// ---------------------------统计更新部分-----------------------------//
void MainWindow::on_ScaleDown_editingFinished()
{
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->yAxis->setRange(0,ui->YScale->value());
    S_cust->replot();
    S_cust->show();
}

void MainWindow::on_Scaleup_editingFinished()
{
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->yAxis->setRange(0,ui->YScale->value());
    S_cust->replot();
    S_cust->show();
}

void MainWindow::on_YScale_editingFinished()
{
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->yAxis->setRange(0,ui->YScale->value());
    S_cust->replot();
    S_cust->show();
}
