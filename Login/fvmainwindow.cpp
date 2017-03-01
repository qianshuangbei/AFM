#include "fvmainwindow.h"
#include "ui_fvmainwindow.h"
#include "myfunction.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include<QStringListModel>

FVMainWindow::FVMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FVMainWindow),
    pixsize(360)

{
     ui->setupUi(this);
     curFile =tr("AZure");
     setWindowTitle(curFile);
     ui->Address->setText("E:\\Graduate List\\CAPTURE\\20170110");
     ui->FrontFileName->setText("20170110.004");
     ui->BackFileName->setText("20170110.008");
     ui->UpBox->setValue(120);
     ui->DownBox->setValue(400);
     ui->ThresholdBox->setValue(0.2);
     ui->ScaleDown->setValue(0);
     ui->Scaleup->setValue(600);
     ui->YScale->setValue(10);
     enDrawing= false;
     m_size = 256;
     n_size = 256;     
     setFocus();
}

FVMainWindow::~FVMainWindow()
{
    delete ui;
}

void FVMainWindow::on_Address_selectionChanged()
{
    QString T_filename;
    T_filename= QFileDialog::getExistingDirectory(this);
    ui->Address->setText(T_filename);
}

void FVMainWindow::on_LoadButton_clicked()
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

void FVMainWindow::on_Meshbtn_clicked()   // Calibration
{
    // retrace部分水平校准
    int xlen=ui->DownBox->value()-ui->UpBox->value();
    int UpBoxval = ui->UpBox->value();
    int *x_matrix = new int [FV_n];
    for(int i=0; i<FV_n; i++)
    {
        x_matrix[i] = i;
    }
    for(int i=0; i<m_size*n_size; i++)
    {
        double curpoly[2];   // curpoly[0] 斜率k curpoly[1] 常数b
        F_polyfit(FV_retrc[i]+UpBoxval, x_matrix+UpBoxval, xlen , curpoly);  //least square
        F_polyCalibration(FV_retrc[i], x_matrix, FV_n, curpoly);   // calibration of least square
        F_polyCalibration(FV_trace[i], x_matrix, FV_n, curpoly);   // calibration of least square

    }

}


void FVMainWindow::on_StatisticButton_clicked()
{
    QVector<double> retrcmin;
    retrcmin.clear();
    for(int i=0; i<m_size*n_size; i++)
    {
        double T= findmin(FV_retrc[i] ,200);
        retrcmin.push_back(-T);
    }
    QMap<int , int >S_table ;
    S_table.clear();
    for(int i=0; i<retrcmin.size();i++)
    {
        int T=retrcmin[i]/0.01;
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
    S_cust->setGeometry(670,340,600,340);
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

void FVMainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString cur= index.data(Qt::DisplayRole).toString();
    loadrowFile(cur);
    loadFVFile(cur);
    enDrawing = true;
    update();


}
//--------------------读取数据部分------------------//
bool FVMainWindow::loadrowFile(const QString &fileName)
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

    else if(curline.contains("\\@4:Z scale: V [Sens. DeflSens]",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("(")+1, 10);
       tmp.trimmed();
       T_defsen= tmp.toDouble();

    }
    else if(curline.contains("@Sens. ZsensSens",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("V")+1, 6);
       tmp.trimmed();
       T_sscale=  tmp.toDouble();
    }
    else if(curline.contains("Spring Constant",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf(":")+1, 8);
       tmp.trimmed();
  //     T_k.push_back(tmp.toDouble());
       T_k.push_back(0.184900);

    }
    else if(curline.contains("@Sens. DeflSens:",Qt::CaseInsensitive)){
       QString tmp =curline.mid( curline.indexOf("V")+1, 6);
       tmp.trimmed();
       T_TMD= tmp.toDouble();

    }
    else if(curline.contains("\\Samps/line:",Qt::CaseInsensitive)){
        curline.remove(0,13);
        curline.trimmed();
        n_size = curline.toInt();
    }
    else if(curline.contains("\\Number of lines:",Qt::CaseInsensitive)){
        curline.remove(0,18);
        curline.trimmed();
        m_size=curline.toInt();
    }



    }
    while(!curline.contains("*File list end"));

    file.close();

    return true;
}

bool FVMainWindow::loaddatFile(const QString &fileName)
{
    N_matrix = new double *[m_size];
    for (int j =0; j<m_size ; j++)
    {
        N_matrix[j] =new double [n_size];
    }
    QFile file(fileName);
    char* a_pData = NULL;
    a_pData = (char*)malloc(T_datalength[1]);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::warning(this, tr("多文档编辑器"),tr("无法写入文件%1：/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    file.seek(T_dataoffset[0]);
    file.read(a_pData, T_datalength[1]);

    int m=0;
    for(int p=0; p<m_size; p++)
    {
        for(int q=0 ; q<n_size; q++)
        {
              QString str1;
              QString str2;
              str1.sprintf("%2.2X", a_pData[2*m]);
              str2.sprintf("%2.2X", a_pData[2*m+1]);
              m++;
              str1 =str1.right(2);
              str2 =str2.right(2);
              str2+=str1;
              if(str2[0]>'7')
              {
                QString s1;
                s1=str2.right(3);
                bool ok;
                int dec =s1.toInt(&ok,16);
                dec=dec-4096;
                N_matrix[p][q]= dec*T_defsen*T_sscale;
              }
              else{
                  bool ok;
                  int dec= str2.toInt(&ok, 16);
                N_matrix[p][q]=dec*T_defsen*T_sscale;
              }
        }
    }
    file.close();
    D_max = N_matrix[0][0];
    D_min = N_matrix[0][0];
    for(int p=0; p<m_size ; p++)
    {
        for(int q=0; q<n_size ;q++ )
        {
            if(N_matrix[p][q]>D_max)
                D_max = N_matrix[p][q];
            else if (N_matrix[p][q]<D_min)
                D_min = N_matrix[p][q];
        }
    }
    return true;

}

bool FVMainWindow::loadFVFile(const QString &fileName)
{

    loaddatFile(fileName);
    FV_n = T_datalength[2]/4/m_size/n_size;
    FV_trace= new double *[m_size*n_size];
    FV_retrc= new double *[m_size*n_size];
    for(int j =0; j<m_size*n_size; j++)
    {
        FV_retrc[j]= new double [FV_n];
        FV_trace[j]= new double [FV_n];
    }
    QFile file(fileName);

    char *a_pData =NULL;
    a_pData =(char*) malloc (T_datalength[2]);
    if(!file.open(QFile::ReadOnly)) return false;
    file.seek( T_dataoffset[1]);
    file.read(a_pData,T_datalength[2]);

    int m=0;
    for(int p=0; p<2*m_size*n_size; p++)
        for(int q=0; q<FV_n; q++)
        {
            QString str1;
            QString str2;
            str1.sprintf("%2.2X", a_pData[2*m]);
            str2.sprintf("%2.2X", a_pData[2*m+1]);
            m++;
            str1 =str1.right(2);
            str2 =str2.right(2);
            str2+=str1;
            int dec;
            if(str2[0]>'7')
            {
              QString s1;
              s1=str2.right(3);
              bool ok;
              dec =s1.toInt(&ok,16);
              dec=dec-4096;

            }
            else{
                bool ok;
                dec= str2.toInt(&ok, 16);

            }
            if(p%2==0)
            FV_trace[p/2][q]=dec*T_defsen*T_TMD*T_k[0];
            else FV_retrc[p/2][q]=dec*T_defsen*T_TMD*T_k[0];
        }
    file.close();
    return true;
}
//--------------------读取数据部分------------------//

//--------------------事件 响应--------------------//

void FVMainWindow::paintEvent(QPaintEvent *)
{
    if(enDrawing)
    {

      tempPix = QPixmap(360,360);
      tempPix.fill(Qt::white);
      QPainter painter(this);
      QPen pen(Qt::black);

      painter.setPen(pen);

      int xoffset=60;
      int yoffset=335;

      pen.setWidth(512/n_size+1);
      int T= pixsize/n_size;
      for (int p=0; p<m_size ; p++ )
      {
          for(int q = 0; q<n_size ; q++)
          {
             int m_rgb;
             m_rgb=(N_matrix[p][q]-D_min)/(D_max-D_min)*255;

             pen.setColor(QColor(m_rgb,m_rgb,m_rgb));
             painter.setPen(pen);
             painter.drawPoint(T*(q)+xoffset,T*(m_size-p-1)+yoffset);

          }
      }

    }

}

void FVMainWindow::mousePressEvent(QMouseEvent *event)
{

    if(enDrawing && event->button()==Qt::LeftButton)
    {
        lastPoint =event->pos();          //上一次的鼠标位置
        int xoffset=45;   //bug zaici
        int yoffset=320;
        int T = pixsize/n_size;
        QVector<double> Po_retrc,Po_trace;
        QVector<double> x;

        int FV_x, FV_y;
        FV_x =(lastPoint.x()-xoffset)/T;
        FV_y =m_size-1-(lastPoint.y()-yoffset)/T;
        if( (FV_x<0||FV_x>n_size) || (FV_y<0||FV_y>m_size) ) return ;
        int FV_num=FV_y*n_size+FV_x;

        double min = FV_retrc[FV_num][0];
        double max = FV_retrc[FV_num][0];
        for(int p =0; p<FV_n-10; p++)
        {
            Po_retrc.push_back(FV_retrc[FV_num][p]);
            Po_trace.push_back(FV_trace[FV_num][p]);
            x.push_back(p);

            min = FV_retrc[FV_num][p]<min?FV_retrc[FV_num][p]:min;
            min = FV_trace[FV_num][p]<min?FV_trace[FV_num][p]:min;

            max = FV_retrc[FV_num][p]>max?FV_retrc[FV_num][p]:max;
            max = FV_trace[FV_num][p]>max?FV_trace[FV_num][p]:max;

        }
        double det= (max-min)*0.2;
        customPlot = new QCustomPlot(this);
        customPlot->setGeometry(530,50,730, 270);
        customPlot->addGraph();
        customPlot->graph(0)->setData(x,Po_retrc);
        customPlot->graph(0)->setPen(QPen(Qt::red));
        customPlot->addGraph();
        customPlot->graph(1)->setData(x,Po_trace);

       //设置坐标轴名称
        customPlot->xAxis->setLabel("Height Sensor(pixel)");
        customPlot->yAxis->setLabel("Force(nN)");

       //设置坐标轴显示范围，否则只能看到默认范围
        customPlot->xAxis->setRange(-20, FV_n+30);
        customPlot->yAxis->setRange(min-det, max+det);
        customPlot->replot();

        customPlot->show();


    }
}

//--------------------事件 响应--------------------//

















// -----------------------统计更新部分------------------------------------//
void FVMainWindow::on_YScale_editingFinished()
{
    S_cust->yAxis->setRange(0,ui->YScale->value());
    S_cust->replot();
    S_cust->show();
}

void FVMainWindow::on_Scaleup_editingFinished()
{
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->replot();
    S_cust->show();
}

void FVMainWindow::on_ScaleDown_editingFinished()
{
    S_cust->xAxis->setRange(ui->ScaleDown->value(),ui->Scaleup->value());
    S_cust->replot();
    S_cust->show();
}
