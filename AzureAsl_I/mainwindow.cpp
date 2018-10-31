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
#include <QtDataVisualization/q3dbars.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
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
 * *偏置
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
    // example : http://doc.qt.io/qt-5/qtdatavisualization-surface-example.html
    Q3DSurface *graph = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(graph);
    //! [0]

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    //! [1]
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);
    //! [1]

    widget->setWindowTitle(QStringLiteral("3D Surface"));

    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Model"));

    QRadioButton *sqrtSinModelRB = new QRadioButton(widget);
    sqrtSinModelRB->setText(QStringLiteral("Height"));
    sqrtSinModelRB->setChecked(false);

    QVBoxLayout *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(sqrtSinModelRB);
    modelGroupBox->setLayout(modelVBox);

    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

    QRadioButton *modeNoneRB = new QRadioButton(widget);
    modeNoneRB->setText(QStringLiteral("No selection"));
    modeNoneRB->setChecked(false);

    QRadioButton *modeItemRB = new QRadioButton(widget);
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(false);

    QRadioButton *modeSliceRowRB = new QRadioButton(widget);
    modeSliceRowRB->setText(QStringLiteral("Row Slice"));
    modeSliceRowRB->setChecked(false);

    QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
    modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
    modeSliceColumnRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
    axisMinSliderX->setMinimum(-10);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    axisMinSliderX->setVisible(false);
    QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    axisMaxSliderX->setVisible(false);
    QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMinSliderZ->setMinimum(-10);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    axisMinSliderZ->setVisible(false);
    QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);
    axisMaxSliderZ->setVisible(false);

    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));

    QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientBtoYPB = new QPushButton(widget);
    gradientBtoYPB->setIcon(QIcon(pm));
    gradientBtoYPB->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientGtoRPB = new QPushButton(widget);
    gradientGtoRPB->setIcon(QIcon(pm));
    gradientGtoRPB->setIconSize(QSize(24, 100));

    QHBoxLayout *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    vLayout->addWidget(modelGroupBox);
    vLayout->addWidget(selectionGroupBox);
//    vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
//    vLayout->addWidget(axisMinSliderX);
//    vLayout->addWidget(axisMaxSliderX);
//    vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
//    vLayout->addWidget(axisMinSliderZ);
//    vLayout->addWidget(axisMaxSliderZ);
    vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(colorGroupBox);

    widget->show();

    SurfaceGraph *modifier = new SurfaceGraph(graph, allChannel.back(), _dataManager->xSize, _dataManager->ySize);

    QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
                     modifier, &SurfaceGraph::enable3DModel);
    QObject::connect(modeNoneRB, &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeNone);
    QObject::connect(modeItemRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeItem);
    QObject::connect(modeSliceRowRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceRow);
    QObject::connect(modeSliceColumnRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceColumn);
    QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustXMin);
    QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustXMax);
    QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustZMin);
    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustZMax);
    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),
                     modifier, SLOT(changeTheme(int)));
    QObject::connect(gradientBtoYPB, &QPushButton::pressed,
                     modifier, &SurfaceGraph::setBlackToYellowGradient);
    QObject::connect(gradientGtoRPB, &QPushButton::pressed,
                     modifier, &SurfaceGraph::setGreenToRedGradient);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);

    sqrtSinModelRB->setChecked(true);
    modeItemRB->setChecked(true);
    themeList->setCurrentIndex(2);

}

void MainWindow::on_actionFlatten_triggered()
{

}
int compare(const void * a, const void * b)
 {
   double da = *(double*)a;
   double db = *(double*)b;
   return (da > db) ? 1 : -1;
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
    double Ksum[Kx];//组数记录
    int ik = 0;
    for(int ix = 0; ix < Size; ix++){
        if(HData[ix] < Lu[knum]) fnum++;
        else{
            Ksum[ik++] = fnum;
            fnum = 1;
            knum++;
        }
    }
    double fresum[Kx];
    for(int ix = 0;ix < Kx;ix++) fresum[ix] = Ksum[ix]  / 65536;
    QFile openDefaultDir("HistogramData.txt");
    QTextStream in(&openDefaultDir);
    openDefaultDir.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    knum = 1;
    for(int ix = 0;ix < Kx;ix++) in <<"The frequency of data's up-limited "<< Lu[ix]<<": "<< fresum[ix]  <<endl;
    openDefaultDir.close();
    GetHistogram(Lu,fresum,Kx,hx);

}
void MainWindow::GetHistogram(double *x, double *y, int Kx, double hx)
{
    ui->CurveImagePlot->setVisible(true);
    QVector<double> xn(Kx);
    QVector<double> yn(Kx);

    for(int ix =0 ;ix < Kx;ix++) {
        xn[ix] = x[ix];
        yn[ix] = y[ix];
    }
    QCPBars *newBars = new QCPBars(ui->CurveImagePlot->xAxis, ui->CurveImagePlot->yAxis);
  // ui->CurveImagePlot->setGeometry(700, 50, 700, 400);

    ui->CurveImagePlot->clearGraphs();
    ui->CurveImagePlot->xAxis->setTickLabels(true);
    newBars->setData(xn,yn);
    newBars->setWidth(hx);
    ui->CurveImagePlot->rescaleAxes(true);

    ui->CurveImagePlot->xAxis->setLabel("the data of height/nm");
    ui->CurveImagePlot->yAxis->setLabel("probability/%");
    ui->CurveImagePlot->setBackground(QColor(50,50,50));
    ui->CurveImagePlot->setInteractions(QCP::iRangeZoom|QCP::iSelectPlottables);
    ui->CurveImagePlot->setSelectionRectMode(QCP::srmZoom);
    ui->CurveImagePlot->replot();

}






    //QCPBars *newBars = new QCPBars(ui->CurveImagePlot->xAxis, ui->CurveImagePlot->yAxis);
 //   for(int ix =0 ;ix < Kx;ix++) newBars->addData(x[ix],y[ix]);
   // ui->CurveImagePlot->setVisible(true);


