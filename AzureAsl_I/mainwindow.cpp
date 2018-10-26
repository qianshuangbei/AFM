#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QIODevice>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QLayout>
#include <QPushButton>
#include<QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QtDataVisualization>
#include "math.h"
#include "plotlines.h"
#include "tool_base.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pressRelease = false;
    _mouseActive = false;
    _isDoubleChannel= false;
/*
 * 偏置
*/
    yOffset = 0;
    xOffset = 0;
    mSize = 256;
    nSize = 256;
    QPalette mainwindowColor(this->palette());
    mainwindowColor.setColor(QPalette::Background, QColor(232, 241, 252));
    this->setPalette(mainwindowColor);
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

}

bool MainWindow::LoadDataFile(const QString &fileName, const int index, double **matrix)
{
    int16_t* pData = NULL;
    pData = (int16_t*)malloc(_dataManager->dataLength[index+1]);

    std::string nanoFileName = fileName.toStdString();
    FILE *nanoFile = fopen(nanoFileName.c_str(), "rb");
    fseek(nanoFile , _dataManager->dataOffset[0], 0);//指针偏移到数据起始点
    fread( pData, 2, _dataManager->dataLength[index+1]/2 , nanoFile);
    double zScale = _dataManager->dataZScale[index];
    double sScale = _dataManager->dataSScale[index];
    for (int p=0; p < mSize ; p++){
        for(int q = 0; q < nSize ; q++){
            matrix[p][q] = pData[p*nSize + q]*(zScale/65536)*sScale;
        }
    }
    free(pData);
    update();
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    // 存储和读取 上一次打开位置
    QFile openDefaultDir("defaultaddress.txt");
    QString defaultDir;
    QTextStream in(&openDefaultDir);
    QTextStream out(&openDefaultDir);
    openDefaultDir.open(QIODevice::ReadWrite|QIODevice::Text);
    out >> defaultDir;
    openDefaultDir.close();

    _fileName = QFileDialog::getOpenFileName(this, tr("Open AFM File"), defaultDir);
    unsigned int i = _fileName.length() - 1;
    for( ; i > 1; i--){
        if(_fileName[i] == '/') break;
    }
    openDefaultDir.open( QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    defaultDir = _fileName == "" ? defaultDir : _fileName.left(i);
    in << defaultDir;
    openDefaultDir.close();

    // 数据实例化

    if(!_fileName.isEmpty()){

        _dataManager = DataManager::Instance();
        _dataManager->LoadRowFile(_fileName);

        mSize = _dataManager->ySize;
        nSize = _dataManager->xSize;
        double **matrix;     //当前数组
        matrix = new double *[mSize];
        for (int j =0; j<mSize ; j++)
        {
            matrix[j] =new double [nSize];
        }

        int index= 0 ;
        for(i = 0; i < _dataManager->type.size(); i++){
            if(_dataManager->type[i] == "Height") {
                index = i;
            }
        }
        if(!allChannel.size()){
            allChannel.push_back(matrix);
            _pastSize.push_back({mSize, nSize});
            InitAfterFile();
        }else{
            for(i = 0; i < _pastSize[0][0]; i++)
                    delete[] allChannel[0][i];
            delete[] allChannel[0];
            allChannel[0] = matrix;
            _pastSize[0][0] = mSize;
            _pastSize[0][1] = nSize;
        }
        LoadDataFile(_fileName, index, allChannel.back());

    // 目前只导入了 Height类型的数据
    //    int Channelnum = 1;
    //    for(int i=1; i<Channelnum; i++){
    //        double** TempMat = new double *[mSize];
    //        for(int j=0; j<mSize ; j++) TempMat[j] = new double [nSize];
    //        allChannel.push_back(TempMat);
    //    }
    //    for(int i=1; i<Channelnum; i++){
    //       LoadDataFile(_fileName,i, allChannel[i]);
    //    }

        ui->actionSection->setEnabled(true);
        ui->actionPower_Spectral_Denstiy->setEnabled(true);

        GenerateMain2DImage();
        GenerateInformation();

    }
}



void MainWindow:: paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QRect mainW = this->geometry();
    int mainWindowWidth = mainW.width();
    int mainWindowHeight = mainW.height();

    QRect mainWindowBorder(0, 61, mainWindowWidth, mainWindowHeight - 64);
    painter.setPen(QPen(QColor(170,170,170,255), 10));
    painter.drawRoundedRect(mainWindowBorder, 0, 0);

    QRect titleBac(5, 61, mainWindowWidth - 10, 24);
    QRect titleBorder(10, 62, mainWindowWidth - 20, 20);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(170, 170, 170, 255));
    painter.drawRoundedRect(titleBac, 0, 0);
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.drawRoundedRect(titleBorder, 0, 0);


    QFont font("Microsoft YaHei", 11);
    font.setBold(20);
    painter.setPen(QPen(QColor(0,0,0)));
    painter.drawText(15,77,"2D Image");

/*
    painter.setPen(QPen(Qt::red, 2));
    if(_pressRelease){
        QPainter custpainter(this);
        custpainter.drawLine(lastPoint.x(),lastPoint.y(),curPoint.x(),curPoint.y());
    }
*/
}

void MainWindow::InitAfterFile(){



    /*
     * 状态栏
    */

    _mouseStartLabel = new QLabel();
    _mouseStartLabel->setText("");
    _mouseStartLabel->setFixedWidth(300);
    statusBar()->addPermanentWidget(_mouseStartLabel);

    _mouseEndLabel = new QLabel();
    _mouseEndLabel->setText("");
    _mouseEndLabel->setFixedWidth(300);
    statusBar()->addPermanentWidget(_mouseEndLabel);

    _mouseMoveLabel = new QLabel();
    _mouseMoveLabel->setText("");
    _mouseMoveLabel->setFixedWidth(300);
    statusBar()->addPermanentWidget(_mouseMoveLabel);


    _mouseActive = true;
    ui->TwoDImagePlot->setMouseTracking(true);

}


void MainWindow::GenerateMain2DImage(){

    ui->TwoDImagePlot->setVisible(true);
    //    ui->TwoDImagePlot->plotLayout()->clear();
    //    ui->TwoDImagePlot->plotLayout()->simplify();

    unsigned int writePart = (nSize- mSize)/(double)nSize*256;
    xOffset = 10;
    yOffset = 40 + writePart;

    ui->TwoDImagePlot-> setGeometry(xOffset+5, yOffset, 660, (double)mSize * 574 / nSize);
    ui->TwoDImagePlot->axisRect()->setupFullAxesBox(false);
    ui->TwoDImagePlot->xAxis->setLabel("Height Sensor");
    ui->TwoDImagePlot->yAxis->setTicks(true);
    ui->TwoDImagePlot->yAxis->setTickLabels(true);

    QCPColorMap *colorMap = new QCPColorMap(ui->TwoDImagePlot->xAxis, ui->TwoDImagePlot->yAxis);
    colorMap->data()->clear();
    colorMap->data()->setSize(nSize, mSize);
    colorMap->data()->setRange(QCPRange(0, nSize), QCPRange(0, mSize));
    double** matrix = allChannel.back();
    for (int xIndex=0; xIndex < mSize; ++xIndex){
      for (int yIndex=0; yIndex < nSize; ++yIndex){
        colorMap->data()->setCell(yIndex, xIndex, matrix[xIndex][yIndex]);
      }
    }
    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(ui->TwoDImagePlot);
    ui->TwoDImagePlot->plotLayout()->addElement(0,1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("nm");
    colorMap->setGradient(QCPColorGradient::gpPolar);
    colorMap->rescaleDataRange();

    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->TwoDImagePlot);
    ui->TwoDImagePlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    ui->TwoDImagePlot->rescaleAxes();
    ui->TwoDImagePlot->replot();

    _pos00 = colorMap->coordsToPixels(0,0);
    _posmn = colorMap->coordsToPixels(nSize,mSize);

    // add user Interactions
    ui->TwoDImagePlot->setInteractions(QCP::iSelectPlottables);
    ui->TwoDImagePlot->setSelectionRectMode(QCP::srmRaphRect);
    ui->TwoDImagePlot->selectionRect()->setPen(QPen(Qt::red));

    connect(ui->TwoDImagePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent *)));
    connect(ui->TwoDImagePlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
    connect(ui->TwoDImagePlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));

    ui->TwoDImagePlot->replot();
}

void MainWindow::GenerateInformation(){


      ui->newtable->setGeometry(10,650,670,200);
      QStringList header;
      header << "" << "" << "" << "";
      ui->newtable->setHorizontalHeaderLabels(header);
      ui->newtable->verticalHeader()->setVisible(false);
      ui->newtable->setItem(0, 0, new QTableWidgetItem("Scan Size"));
      ui->newtable->setItem(1, 0, new QTableWidgetItem("Scan Rate"));
      ui->newtable->setItem(2, 0, new QTableWidgetItem("Samples/Line"));
      ui->newtable->setItem(3, 0, new QTableWidgetItem("Lines"));
      ui->newtable->setItem(4, 0, new QTableWidgetItem("Line Direction"));
      ui->newtable->setItem(0, 2, new QTableWidgetItem("Date"));
      ui->newtable->setItem(1, 2, new QTableWidgetItem("Aspect Rato"));
      ui->newtable->setItem(2, 2, new QTableWidgetItem("Capture Direction"));
      ui->newtable->setItem(3, 2, new QTableWidgetItem("Amplitude Setpoint"));
      ui->newtable->setItem(4, 2, new QTableWidgetItem("Drive Amplitude"));

      ui->newtable->setItem(0, 1, new QTableWidgetItem(_dataManager->scanSize));
      ui->newtable->setItem(1, 1, new QTableWidgetItem(_dataManager->rate));
      ui->newtable->setItem(2, 1, new QTableWidgetItem(nSize));
      ui->newtable->setItem(3, 1, new QTableWidgetItem(mSize));
      ui->newtable->setItem(4, 1, new QTableWidgetItem(_dataManager->lineDirection));
      ui->newtable->setItem(0, 3, new QTableWidgetItem(_dataManager->date));
      ui->newtable->setItem(1, 3, new QTableWidgetItem(_dataManager->ampSetPoint));
      ui->newtable->setItem(2, 3, new QTableWidgetItem(_dataManager->captureDirection));
      ui->newtable->setItem(3, 3, new QTableWidgetItem(_dataManager->ratio));
      ui->newtable->setItem(4, 3, new QTableWidgetItem(_dataManager->driveAmp));

      ui->newtable->resizeColumnsToContents();
      ui->newtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->newtable->setVisible(true);
}


void MainWindow::on_actionSection_triggered()
{
    ui->TwoDImagePlot->setSelectionRectMode(QCP::srmRaphLine);
    ui->CurveImagePlot->setGeometry(700, 50, 700, 400);
    ui->CurveImagePlot->clearGraphs();
    ui->CurveImagePlot->replot();
    ui->CurveImagePlot->xAxis2->setVisible(true);
    ui->CurveImagePlot->xAxis2->setTickLabels(false);
    ui->CurveImagePlot->yAxis2->setVisible(true);
    ui->CurveImagePlot->yAxis2->setTickLabels(false);
    ui->CurveImagePlot->setInteractions( QCP::iRangeZoom | QCP::iSelectPlottables| QCP::iRangeDrag);
     ui->CurveImagePlot->setVisible(true);
    if(_isDoubleChannel){
        ui->CurveImageIIPlot->setGeometry(700, 450, 700, 400);
        ui->CurveImageIIPlot->xAxis2->setVisible(true);
        ui->CurveImageIIPlot->xAxis2->setTickLabels(false);
        ui->CurveImageIIPlot->yAxis2->setVisible(true);
        ui->CurveImageIIPlot->yAxis2->setTickLabels(false);
        ui->CurveImageIIPlot->setInteractions( QCP::iRangeZoom | QCP::iSelectPlottables| QCP::iRangeDrag);

    }
}

void MainWindow::SectionAddData(int x1, int y1, int x2, int y2){


    ToolBase *crossLine = new ToolBase();
    crossLine->imageXOffset= _pos00.x();
    crossLine->imageYOffset= _pos00.y();
    crossLine->imageXLength= qAbs(_pos00.x()-_posmn.x());
    crossLine->imageYLength= qAbs(_pos00.y()-_posmn.y());
    crossLine->ySize = mSize;
    crossLine->xSize = nSize;


    QVector<QVector<double> > poLine;
    poLine= crossLine->ToolCrossLine(allChannel.back(), x1, y1, x2, y2);
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->graph(0)->setPen(QPen(Qt::blue));
    ui->CurveImagePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(0)->setData(poLine[0], poLine[1]);
    ui->CurveImagePlot->graph(0)->rescaleAxes(true);
    ui->CurveImagePlot->replot();

    if(_isDoubleChannel){
        _pos00 = _doubleColorMap->coordsToPixels(0,0);
        _posmn = _doubleColorMap->coordsToPixels(nSize,mSize);

        crossLine->imageXOffset= _pos00.x();
        crossLine->imageYOffset= _pos00.y();
        crossLine->imageXLength= qAbs(_pos00.x()-_posmn.x());
        crossLine->imageYLength= qAbs(_pos00.y()-_posmn.y());
        crossLine->ySize = mSize;
        crossLine->xSize = nSize;


        QVector<QVector<double> > poLine;
        poLine= crossLine->ToolCrossLine(allChannel.back(), x1, y1, x2, y2);
        ui->CurveImageIIPlot->addGraph();
        ui->CurveImageIIPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->CurveImageIIPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->CurveImageIIPlot->graph(0)->setData(poLine[0], poLine[1]);
        ui->CurveImageIIPlot->graph(0)->rescaleAxes(true);
        ui->CurveImageIIPlot->replot();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(!_mouseActive)
        return;
    QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
    lastPoint= event->pos();
    _mouseStartLabel->setText("Mouse Start Position:"+str);
    _pressRelease = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(!_mouseActive)
        return;
     QString str = "(" + QString::number(event->x()) + ","+QString::number(event->y()) + ")";
     endPoint= event->pos();
     _mouseEndLabel->setText("Mouse End Position:"+str);
     SectionAddData(lastPoint.x() , lastPoint.y(), endPoint.x(), endPoint.y());
     _pressRelease = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(!_mouseActive)
        return;
    QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
    _mouseMoveLabel->setText("Mouse Current Postion:"+str);
    curPoint=event->pos();
    if(_pressRelease)
        update();

}

void MainWindow::on_actionRoughness_triggered()
{

}

void MainWindow::on_actionPower_Spectral_Denstiy_triggered()
{
    ui->CurveImagePlot->setGeometry(700, 50, 700, 400);

    ToolBase *psd = new ToolBase();
    psd->xSize= nSize;
    psd->ySize= mSize;
    psd->xScale=_dataManager->dataSScale[0];
    QVector<QVector<double> >psdResH = psd->ToolHPowerSpectralDensity(allChannel.back());
    QVector<QVector<double> >psdResV = psd->ToolVPowerSpectralDensity(allChannel.back());

    ui->CurveImagePlot->clearGraphs();
    ui->CurveImagePlot->replot();
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->legend->setVisible(true);
    ui->CurveImagePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->CurveImagePlot->graph(0)->setPen(QPen(Qt::blue));
    ui->CurveImagePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(0)->setData(psdResH[0], psdResH[1]);
    ui->CurveImagePlot->graph(0)->setName("X Orientation");
    ui->CurveImagePlot->addGraph();
    ui->CurveImagePlot->graph(1)->setPen(QPen(Qt::red));
    ui->CurveImagePlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->CurveImagePlot->graph(1)->setData(psdResV[0], psdResV[1]);
    ui->CurveImagePlot->graph(1)->setName("Y Orientation");
    ui->CurveImagePlot->graph(0)->rescaleAxes(true);
    ui->CurveImagePlot->graph(1)->rescaleAxes(true);

    ui->CurveImagePlot->xAxis->setLabel("Normalized X&Y Frequency(log nm^-1)");
    ui->CurveImagePlot->yAxis->setLabel("psd(log nm3)");

    ui->CurveImagePlot->setInteractions(QCP::iRangeZoom|QCP::iSelectPlottables);
    ui->CurveImagePlot->setSelectionRectMode(QCP::srmZoom);
    ui->CurveImagePlot->selectionRect()->setPen(QPen(Qt::red));
    ui->CurveImagePlot->replot();
}

void MainWindow::on_actionDouble_Channel_triggered()
{
    ui->TwoDImagePlot->setGeometry(15,40,660,(double)mSize*574/nSize);
    ui->TwoDImageIIPlot->setGeometry(15, 40+(double)mSize*574/nSize, 660,(double)mSize*574/nSize);
    ui->TwoDImageIIPlot->axisRect()->setupFullAxesBox(true);
    ui->TwoDImageIIPlot->xAxis->setLabel("Height Sensor");
    ui->TwoDImageIIPlot->yAxis->setTicks(false);
    ui->TwoDImageIIPlot->yAxis->setTickLabels(false);

    _doubleColorMap = new QCPColorMap(ui->TwoDImageIIPlot->xAxis, ui->TwoDImageIIPlot->yAxis);
    _doubleColorMap->data()->setSize(nSize,mSize);
    _doubleColorMap->data()->setRange(QCPRange(0,nSize), QCPRange( 0,mSize));
    double** matrix = allChannel.back();
    for (int xIndex=0; xIndex<mSize; ++xIndex){
      for (int yIndex=0; yIndex<nSize; ++yIndex){
        _doubleColorMap->data()->setCell(yIndex,xIndex,  matrix[xIndex][yIndex]);
      }
    }

    QCPColorScale *colorScale = new QCPColorScale(ui->TwoDImageIIPlot);
    ui->TwoDImageIIPlot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    _doubleColorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("nm");
    _doubleColorMap->setGradient(QCPColorGradient::gpPolar);
    _doubleColorMap->rescaleDataRange();
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->TwoDImageIIPlot);
    ui->TwoDImageIIPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    ui->TwoDImageIIPlot->rescaleAxes();
    ui->TwoDImageIIPlot->replot();

    _isDoubleChannel = true;
}

void MainWindow::on_actionThree_Dimension_triggered()
{


}

void MainWindow::on_actionFlatten_triggered()
{

}
int compare(const void * a, const void * b)
 {
   return ( *(double*)a - *(double*)b );
 }

void MainWindow::on_actionHistogram_triggered()
{
    double** matrix = allChannel.back();
    int Size = nSize*mSize;
    double HData[Size];
    int tempi = 0;
    for(int ix = 0;ix < mSize; ix++){
        for(int iy = 0;iy < nSize; iy++){
            HData[tempi++] = matrix[ix][iy];
        }
    }
    qsort(HData, Size, sizeof(double),compare);//生序排列
    double Dmax = HData[Size-1];
    double Dmin = HData[0];
    double Rx = Dmax - Dmin;//求出极差
    int Kx = 2 + 3.32*log10(Size); //史特吉斯组数经验公式
    double hx = Rx / Kx;//求组距
    double Lu[Kx];
    double Ld[Kx];
    Ld[0] = HData[0];
    for(int i = 0;i < Kx; i++){
        Lu[i] = Ld[i] + hx;
        Ld[i+1] = Lu[i];
    }//求出各组上下限
    int fnum = 0;//次数统计
    int knum = 0;//组号
    int Ksum[Kx];//组数记录
    int ik = 0;
    for(int ix = 0; ix < Size; ix++){
        if(HData[ix] < Lu[knum]) fnum++;
        else{
            Ksum[ik++] = fnum;
            fnum = 1;
            knum++;
        }
    }

}
