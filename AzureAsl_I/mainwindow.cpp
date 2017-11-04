#include "mainwindow.h"
#include "plotlines.h"
#include "tool_base.h"


#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QIODevice>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QLayout>
#include <QPushButton>
#include "math.h"
#include<QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    W_NEW = false;
    PRESS_RELEASE = false;
    MouseActive = false;
    isDoubleChannel= false;
/*
 * 偏置
*/
    yoffset = 0;
    xoffset = 0;
    m_size = 256;
    n_size = 256;
    QPalette mainwindow_color(this->palette());
    mainwindow_color.setColor(QPalette::Background, QColor(232, 241, 252));
    this->setPalette(mainwindow_color);

/*
 * 失能
*/
    ui->actionSection->setDisabled(true);
    ui->actionRoughness->setDisabled(true);

/*
 * 初始化隐藏部件
*/



    ui->newtable->setVisible(false);
    ui->TwoDImagePlot->setVisible(false);
    ui->CurveImagePlot->setVisible(false);
    ui->TwoDImageIIPlot->setVisible(false);
    ui->CurveImageIIPlot->setVisible(false);
    ui->newtable->setGeometry(-1,-1,-1,-1);



}

MainWindow::~MainWindow()
{
  /*   释放 有点问题
    for (int i=0 ; i<m_size; i++)
    {
        delete [] N_matrix[i];
    }
    delete[] N_matrix;
    delete ui;
    */
}

bool MainWindow::LoadDataFile(const QString &fileName, const int Ti, double **matrix)
{

    int16_t* a_pData = NULL;
    a_pData = (int16_t*)malloc(rowInfoZ.r_length[Ti+1]);

    int16_t **xmatrix;
    xmatrix = new int16_t *[m_size];
    for (int j =0; j<m_size ; j++)    {
        xmatrix[j] =new int16_t [n_size];
    }
    std::string nanofilename = fileName.toStdString();
    FILE *nanofile = fopen(nanofilename.c_str(), "rb");
    fseek(nanofile , rowInfoZ.r_offset[0], 0);
    fread( a_pData, 2, rowInfoZ.r_length[Ti+1]/2 , nanofile);
    for(int i=0; i<m_size; i++){
        xmatrix[i] = &a_pData[i*n_size];
    }
    for (int p=0; p<m_size ; p++ ){
        for(int q = 0; q<n_size ; q++){
            N_matrix[p][q] = xmatrix[p][q]*(rowInfoZ.r_zscale[Ti]/65536)*rowInfoZ.r_sscale;
        }
    }

/*
    for (int i=0 ; i<m_size; i++){
        delete [] xmatrix[i];
    }
    delete[] xmatrix;
*/
    free(a_pData);
    update();
    return true;
}



void MainWindow::on_actionOpen_triggered()
{

/*
 * 閰嶇疆姣忔鎵撳紑杞欢鐨勯粯璁よ矾寰
 *
*/
    if(!W_NEW){
        QFile opendefaultdir("defaultaddress.txt");
        QString defaultdir;
        QTextStream in(&opendefaultdir);
        QTextStream out(&opendefaultdir);
        opendefaultdir.open( QIODevice::ReadWrite|QIODevice::Text);
        out>>defaultdir;
        opendefaultdir.close();
        p_filename= QFileDialog::getOpenFileName(this, tr("Open AFM File"), defaultdir);
        unsigned int i=p_filename.length()-1;
        for(;i>1; i--)  if(p_filename[i]=='/') break;
        opendefaultdir.open( QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate);
        defaultdir= p_filename==""?defaultdir: p_filename.left(i);
        in<<defaultdir;
        opendefaultdir.close();

    /*
     * 璇诲彇鏂囦欢鍐呭
    */
        if(!p_filename.isEmpty()){
            bool ok;
            rowInfoZ.loadinfo(p_filename);

            m_size = rowInfoZ.r_line.toInt(&ok, 10);
            n_size = rowInfoZ.r_samlpel.toInt(&ok, 10);

            N_matrix = new double *[m_size];
            for (int j =0; j<m_size ; j++)
            {
                N_matrix[j] =new double [n_size];
        }
        ALL_mat.push_back(N_matrix);
        int Channelnum = rowInfoZ.r_type.size();
        for(int i=1; i<Channelnum; i++){
            double** TempMat = new double *[m_size];
            for(int j=0; j<m_size ; j++) TempMat[j] = new double [n_size];
            ALL_mat.push_back(TempMat);
        }
        W_NEW= true;
        LoadDataFile(p_filename,0, N_matrix);
        for(int i=1; i<Channelnum; i++){
           LoadDataFile(p_filename,i, ALL_mat[i]);
        }

        ui->actionSection->setEnabled(true);
        ui->actionPower_Spectral_Denstiy->setEnabled(true);
        InitAfterFile();
        GenerateMain2DImage();
        GenerateInformation();

        }
    }
    else {
        new_window = new MainWindow(this);
        new_window->show();
    }
}



void MainWindow:: paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /*
     * 涓荤晫闈㈣竟妗
     */
/*
    QRect MAINW =  this->geometry();
    int mainwindowwidth = MAINW.width();
    int mainwindowheight = MAINW.height();

    QRect mainwindowborder(0,61,mainwindowwidth,mainwindowheight-64);
    painter.setPen(QPen(QColor(170,170,170,255), 10));
    painter.drawRoundedRect(mainwindowborder,0,0);

    QRect titlebac(5,61,mainwindowwidth-10,24);
    QRect titleborder(10,62,mainwindowwidth-20,20);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(170,170,170,255));
    painter.drawRoundedRect(titlebac, 0,0);
    painter.setPen(QPen(QColor(60,60,60), 2));
    painter.drawRoundedRect(titleborder, 0,0);


    QFont font("Microsoft YaHei", 11);
    font.setBold(20);
    painter.setPen(QPen(QColor(0,0,0)));
    painter.drawText(15,77,"2D Image");
    */
/*
    painter.setPen(QPen(Qt::red, 2));
    if(PRESS_RELEASE){
        QPainter custpainter(this);
        custpainter.drawLine(lastPoint.x(),lastPoint.y(),curPoint.x(),curPoint.y());
    }
*/
}

void MainWindow::InitAfterFile(){



    /*
     * 状态栏
    */

    mouseStartLabel = new QLabel();
    mouseStartLabel->setText("");
    mouseStartLabel->setFixedWidth(300);

    mouseEndLabel = new QLabel();
    mouseEndLabel->setText("");
    mouseEndLabel->setFixedWidth(300);

    mouseMoveLabel = new QLabel();
    mouseMoveLabel->setText("");
    mouseMoveLabel->setFixedWidth(300);

    statusBar()->addPermanentWidget(mouseStartLabel);
    statusBar()->addPermanentWidget(mouseEndLabel);
    statusBar()->addPermanentWidget(mouseMoveLabel);

    MouseActive = true;
    ui->TwoDImagePlot->setMouseTracking(true);

}


void MainWindow::GenerateMain2DImage(){

    ui->TwoDImagePlot->setVisible(true);


    unsigned int writepart = (n_size- m_size)/(double)n_size*256;
    xoffset = 10;
    yoffset = 40+writepart;

    ui->TwoDImagePlot-> setGeometry(xoffset+5,yoffset,660,(double)m_size*574/n_size);
    ui->TwoDImagePlot->axisRect()->setupFullAxesBox(true);
    ui->TwoDImagePlot->xAxis->setLabel("Height Sensor");
    ui->TwoDImagePlot->yAxis->setTicks(false);
    ui->TwoDImagePlot->yAxis->setTickLabels(false);

    colorMap = new QCPColorMap(ui->TwoDImagePlot->xAxis, ui->TwoDImagePlot->yAxis);

    colorMap->data()->setSize( n_size,m_size);
    colorMap->data()->setRange(QCPRange(0,n_size), QCPRange( 0,m_size));

    double** matrix = ALL_mat[0];
    for (int xIndex=0; xIndex<m_size; ++xIndex){
      for (int yIndex=0; yIndex<n_size; ++yIndex){
        colorMap->data()->setCell(yIndex,xIndex,  matrix[xIndex][yIndex]);
      }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(ui->TwoDImagePlot);
    ui->TwoDImagePlot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("nm");
    colorMap->setGradient(QCPColorGradient::gpPolar);
    colorMap->rescaleDataRange();
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->TwoDImagePlot);
    ui->TwoDImagePlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    ui->TwoDImagePlot->rescaleAxes();
    ui->TwoDImagePlot->replot();

    // add user Interactions
    ui->TwoDImagePlot->setInteractions(QCP::iSelectPlottables);
    ui->TwoDImagePlot->setSelectionRectMode(QCP::srmRaphRect);
    ui->TwoDImagePlot->selectionRect()->setPen(QPen(Qt::red));

    connect(ui->TwoDImagePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent *)));
    connect(ui->TwoDImagePlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
    connect(ui->TwoDImagePlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));



}

void MainWindow::GenerateInformation(){


      ui->newtable->setGeometry(10,650,670,200);
      QStringList header;
      header<<""<<""<<""<<"";
      ui->newtable->setHorizontalHeaderLabels(header);
      ui->newtable->verticalHeader()->setVisible(false);
      ui->newtable->setItem(0,0,new QTableWidgetItem("Scan Size"));
      ui->newtable->setItem(1,0,new QTableWidgetItem("Scan Rate"));
      ui->newtable->setItem(2,0,new QTableWidgetItem("Samples/Line"));
      ui->newtable->setItem(3,0,new QTableWidgetItem("Lines"));
      ui->newtable->setItem(4,0,new QTableWidgetItem("Line Direction"));
      ui->newtable->setItem(0,2,new QTableWidgetItem("Date"));
      ui->newtable->setItem(1,2,new QTableWidgetItem("Aspect Rato"));
      ui->newtable->setItem(2,2,new QTableWidgetItem("Capture Direction"));
      ui->newtable->setItem(3,2,new QTableWidgetItem("Amplitude Setpoint"));
      ui->newtable->setItem(4,2,new QTableWidgetItem("Drive Amplitude"));

      ui->newtable->setItem(0,1,new QTableWidgetItem(rowInfoZ.r_SS[0]));
      ui->newtable->setItem(1,1,new QTableWidgetItem(rowInfoZ.r_rate));
      ui->newtable->setItem(2,1,new QTableWidgetItem(rowInfoZ.r_samlpel));
      ui->newtable->setItem(3,1,new QTableWidgetItem(rowInfoZ.r_line));
      ui->newtable->setItem(4,1,new QTableWidgetItem(rowInfoZ.r_LD));
      ui->newtable->setItem(0,3,new QTableWidgetItem(rowInfoZ.r_date));
      ui->newtable->setItem(1,3,new QTableWidgetItem(rowInfoZ.r_AR));
      ui->newtable->setItem(2,3,new QTableWidgetItem(rowInfoZ.r_CD));
      ui->newtable->setItem(3,3,new QTableWidgetItem(rowInfoZ.r_AS));
      ui->newtable->setItem(4,3,new QTableWidgetItem(rowInfoZ.r_DA));

      ui->newtable->resizeColumnsToContents();
      ui->newtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->newtable->setVisible(true);








}


void MainWindow::on_actionSection_triggered()
{
    ui->TwoDImagePlot->setSelectionRectMode(QCP::srmRaphLine);

    ui->CurveImagePlot->setGeometry(700,50,700,400);
    ui->CurveImagePlot->clearGraphs();
    ui->CurveImagePlot->replot();
    ui->CurveImagePlot->xAxis2->setVisible(true);
    ui->CurveImagePlot->xAxis2->setTickLabels(false);
    ui->CurveImagePlot->yAxis2->setVisible(true);
    ui->CurveImagePlot->yAxis2->setTickLabels(false);
    ui->CurveImagePlot->setInteractions( QCP::iRangeZoom | QCP::iSelectPlottables| QCP::iRangeDrag);
     ui->CurveImagePlot->setVisible(true);
    if(isDoubleChannel){
        ui->CurveImageIIPlot->setGeometry(700, 450, 700,400);
        ui->CurveImageIIPlot->xAxis2->setVisible(true);
        ui->CurveImageIIPlot->xAxis2->setTickLabels(false);
        ui->CurveImageIIPlot->yAxis2->setVisible(true);
        ui->CurveImageIIPlot->yAxis2->setTickLabels(false);
        ui->CurveImageIIPlot->setInteractions( QCP::iRangeZoom | QCP::iSelectPlottables| QCP::iRangeDrag);

    }

}

void MainWindow::SectionAddData(int x1, int y1, int x2, int y2){

    QPointF pos_00 = colorMap->coordsToPixels(0,0);
    QPointF pos_mn = colorMap->coordsToPixels(n_size,m_size);
    Tool_base *crossline1 = new Tool_base();
    crossline1->imagexoffset= pos_00.x();
    crossline1->imageyoffset= pos_00.y();
    crossline1->imagexlength= qAbs(pos_00.x()-pos_mn.x());
    crossline1->imageylength= qAbs(pos_00.y()-pos_mn.y());
    crossline1->y_size = m_size;
    crossline1->x_size = n_size;


    QVector<QVector<double> > Po_line;
    Po_line= crossline1->Tool_CrossLine(ALL_mat[0] ,x1,y1,x2,y2);
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->graph(0)->setPen(QPen(Qt::blue));
    ui->CurveImagePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(0)->setData(Po_line[0], Po_line[1]);
    ui->CurveImagePlot->graph(0)->rescaleAxes(true);
    ui->CurveImagePlot->replot();

    if(isDoubleChannel){
        pos_00 = DoubleColorMap->coordsToPixels(0,0);
        pos_mn = DoubleColorMap->coordsToPixels(n_size,m_size);

        crossline1->imagexoffset= pos_00.x();
        crossline1->imageyoffset= pos_00.y();
        crossline1->imagexlength= qAbs(pos_00.x()-pos_mn.x());
        crossline1->imageylength= qAbs(pos_00.y()-pos_mn.y());
        crossline1->y_size = m_size;
        crossline1->x_size = n_size;


        QVector<QVector<double> > Po_line;
        Po_line= crossline1->Tool_CrossLine(ALL_mat[5] ,x1,y1,x2,y2);
        ui->CurveImageIIPlot->addGraph();
        ui->CurveImageIIPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->CurveImageIIPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->CurveImageIIPlot->graph(0)->setData(Po_line[0], Po_line[1]);
        ui->CurveImageIIPlot->graph(0)->rescaleAxes(true);
        ui->CurveImageIIPlot->replot();
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event){
if(!MouseActive) return ;
    QString str = "("+ QString::number(event->x())+","+QString::number(event->y())+")";
    lastPoint= event->pos();
    mouseStartLabel->setText("Mouse Start Position:"+str);
    PRESS_RELEASE = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
if(!MouseActive) return ;
     QString str = "("+ QString::number(event->x())+","+QString::number(event->y())+")";
     endPoint= event->pos();
     mouseEndLabel->setText("Mouse End Position:"+str);
     SectionAddData(lastPoint.x() , lastPoint.y(), endPoint.x(), endPoint.y());
     PRESS_RELEASE = false;

}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
if(!MouseActive) return ;
    QString str = "("+ QString::number(event->x())+","+QString::number(event->y())+")";
    mouseMoveLabel->setText("Mouse Current Postion:"+str);
    curPoint=event->pos();
    if(PRESS_RELEASE) update();

}

void MainWindow::on_actionRoughness_triggered()
{

}

void MainWindow::on_actionPower_Spectral_Denstiy_triggered()
{
    ui->CurveImagePlot->setGeometry(700,50,700,400);

    Tool_base *PSD = new Tool_base();
    PSD->x_size= n_size;
    PSD->y_size= m_size;
    PSD->x_scale=rowInfoZ.r_SS[0].toFloat();
    QVector<QVector<double> >PSDResH = PSD->Tool_HPowerSpectralDensity(N_matrix);
    QVector<QVector<double> >PSDResV = PSD->Tool_VPowerSpectralDensity(N_matrix);

    ui->CurveImagePlot->clearGraphs();
    ui->CurveImagePlot->replot();
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->legend->setVisible(true);
    ui->CurveImagePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->CurveImagePlot->graph(0)->setPen(QPen(Qt::blue));
    ui->CurveImagePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(0)->setData(PSDResH[0], PSDResH[1]);
    ui->CurveImagePlot->graph(0)->setName("X Orientation");
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->graph(1)->setPen(QPen(Qt::red));
    ui->CurveImagePlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(1)->setData(PSDResV[0], PSDResV[1]);
    ui->CurveImagePlot->graph(1)->setName("Y Orientation");
    ui->CurveImagePlot->graph(0)->rescaleAxes(true);
    ui->CurveImagePlot->graph(1)->rescaleAxes(true);


    ui->CurveImagePlot->xAxis->setLabel("Normalized X&Y Frequency(log nm^-1)");
    ui->CurveImagePlot->yAxis->setLabel("PSD(log nm3)");

    ui->CurveImagePlot->setInteractions(QCP::iRangeZoom|QCP::iSelectPlottables);
    ui->CurveImagePlot->setSelectionRectMode(QCP::srmZoom);
//    QCPSelectionRect *R_SelectionRect = new QCPSelectionRect(ui->CurveImagePlot);
//    ui->CurveImagePlot->setSelectionRect(R_SelectionRect);
    ui->CurveImagePlot->selectionRect()->setPen(QPen(Qt::red));


    ui->CurveImagePlot->replot();

}

void MainWindow::on_actionDouble_Channel_triggered()
{
    ui->TwoDImagePlot->setGeometry(15,40,660,(double)m_size*574/n_size);
    ui->TwoDImageIIPlot->setGeometry(15, 40+(double)m_size*574/n_size, 660,(double)m_size*574/n_size);
    ui->TwoDImageIIPlot->axisRect()->setupFullAxesBox(true);
    ui->TwoDImageIIPlot->xAxis->setLabel("Height Sensor");
    ui->TwoDImageIIPlot->yAxis->setTicks(false);
    ui->TwoDImageIIPlot->yAxis->setTickLabels(false);

    DoubleColorMap = new QCPColorMap(ui->TwoDImageIIPlot->xAxis, ui->TwoDImageIIPlot->yAxis);
    DoubleColorMap->data()->setSize(n_size,m_size);
    DoubleColorMap->data()->setRange(QCPRange(0,n_size), QCPRange( 0,m_size));
    double** matrix = ALL_mat[5];
    for (int xIndex=0; xIndex<m_size; ++xIndex){
      for (int yIndex=0; yIndex<n_size; ++yIndex){
        DoubleColorMap->data()->setCell(yIndex,xIndex,  matrix[xIndex][yIndex]);
      }
    }
    QCPColorScale *colorScale = new QCPColorScale(ui->TwoDImageIIPlot);
    ui->TwoDImageIIPlot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    DoubleColorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("nm");
    DoubleColorMap->setGradient(QCPColorGradient::gpPolar);
    DoubleColorMap->rescaleDataRange();
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->TwoDImageIIPlot);
    ui->TwoDImageIIPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    ui->TwoDImageIIPlot->rescaleAxes();
    ui->TwoDImageIIPlot->replot();

    isDoubleChannel = true;



}
