#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGui();
    connect(this->device.getSensor(), SIGNAL(sensorStateChanged()), this, SLOT(handleSensorStateChange()));
    connect(ui->test, SIGNAL(released()), this, SLOT (test()));

    powerState=false;
    changePower();
    connect(ui->PowerButton, &QPushButton::released, this, &MainWindow::changePower);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  initialize basic gui interface
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::initGui()
{
    this->sensorLightOn = true;
    ui->Sensor->setStyleSheet("QLabel {background-color: pink}");
}

/**
  when Sensor class sends signal that the heart monitor has turned off or on, then this function
    updates the sensor gui to the appropraite colour (ON = pink, OFF = grey)
  changes its its sensorLightOn state to true or false accordingly
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::handleSensorStateChange()
{
    qInfo() << "Inside handleSensorStateChange()";

    QLabel *sensor = ui->Sensor;

    if (this->sensorLightOn == true)
    {
        sensor->setStyleSheet("QLabel {background-color: grey}");
        this->sensorLightOn = false;
    }
    else
    {
        sensor->setStyleSheet("QLabel {background-color: pink}");
        this->sensorLightOn = true;
    }

}

void MainWindow::test()
{
    // add temp graph
    qInfo("test");

    QCustomPlot *customPlot = ui->Graph;
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));

    QVector<double> x(251), y(251);
    for (int i=0; i < 251; ++i)
    {
        x[i] = i;
        y[i] = qExp(-i/150.0)*qCos(i/10.0);
    }

    customPlot->xAxis2->setVisible(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(false);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(false);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    customPlot->graph(0)->setData(x, y);
    customPlot->rescaleAxes();

    customPlot->replot();

    // test heart monitor sensor
    qInfo() << "Testing sensor change";
    this->device.getSensor()->changeSensorState(!this->sensorLightOn);

}

void MainWindow::changeBatteryLevel(double newLevel) {

        int newLevelInt = int(newLevel);
        ui->BatteryBar->setValue(newLevelInt);

        if (newLevelInt <= 20) {
            ui->BatteryBar->setStyleSheet("QProgressBar{selection-background-color:#e60000}");
        }

}


void MainWindow::changePower()
{
    powerState = !powerState;
    ui->MenuButton->setEnabled(powerState);
    ui->SelectButton->setEnabled(powerState);
    ui->UpButton->setEnabled(powerState);
    ui->RightButton->setEnabled(powerState);
    ui->LeftButton->setEnabled(powerState);
    ui->DownButton->setEnabled(powerState);
    ui->SelectButton->setEnabled(powerState);
    ui->Screen->setVisible(powerState);
}

