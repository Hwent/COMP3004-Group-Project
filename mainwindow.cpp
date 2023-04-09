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
    connect(this->device.getBattery(), SIGNAL(batteryLevelUpdated()), this, SLOT(handleBatteryChange()));
    connect(ui->test, SIGNAL(released()), this, SLOT (test()));

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
    // default sensor state
    this->sensorLightOn = true;
    ui->Sensor->setStyleSheet("QLabel {background-color: pink}");

    // disable screen and buttons until power is ON
    powerState=false;
    ui->MenuButton->setEnabled(powerState);
    ui->SelectButton->setEnabled(powerState);
    ui->UpButton->setEnabled(powerState);
    ui->RightButton->setEnabled(powerState);
    ui->LeftButton->setEnabled(powerState);
    ui->DownButton->setEnabled(powerState);
    ui->SelectButton->setEnabled(powerState);
    ui->Screen->setVisible(powerState);

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

/**
  when Battery class sends signal that the battery had changed, then this function
    updates the battery gui progress bar
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::handleBatteryChange()
{
    qreal batteryLevel = this->device.getBattery()->getBatteryLevel();
    QProgressBar *batteryBar = ui->BatteryBar;
    QWidget *screen = ui->Screen;

    batteryBar->setValue(batteryLevel);
    if (batteryLevel <= 20)
    {
        batteryBar->setStyleSheet("QProgressBar::chunk {background-color: red}");
        QLabel *lowBatteryMessage = new QLabel("LOW BATTERY", screen);
        QVBoxLayout *layout = new QVBoxLayout(screen);
        layout->addWidget(lowBatteryMessage);
        layout->setAlignment(lowBatteryMessage, Qt::AlignHCenter | Qt::AlignTop);
        screen->setLayout(layout);
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

    // test battery
//    qInfo() << "Testing battery: set to 80%";
//    this->device.getBattery()->updateBatteryLevel(80);
    qInfo() << "Testing battery: set to 15%";
    this->device.getBattery()->updateBatteryLevel(15);


}

void MainWindow::changeBatteryLevel(double newLevel) {

        int newLevelInt = int(newLevel);
        ui->BatteryBar->setValue(newLevelInt);

        if (newLevelInt <= 20) {
            ui->BatteryBar->setStyleSheet("QProgressBar{selection-background-color:#e60000}");
        }

}

/**
  disable/enable all buttons and screen if device is ON/OFF
  @param {}
  @return {void} Returns nothing
*/
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

