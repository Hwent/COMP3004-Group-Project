#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGui();

    connect(this->device.getSensor(), SIGNAL(sensorStateChanged()), this, SLOT(handleSensorStateChange()));
    connect(this->device.getBattery(), SIGNAL(batteryLevelUpdated()), this, SLOT(handleBatteryChange()));
    connect(ui->MenuButton, SIGNAL (released()), this, SLOT (menuButtonPressed()));
    connect(ui->DownButton, SIGNAL (released()), this, SLOT (downArrowPressed()));
    connect(ui->UpButton, SIGNAL (released()), this, SLOT (upArrowPressed()));
    connect(ui->SelectButton, SIGNAL (released()), this, SLOT (selectorButtonPressed()));
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
    this->isMenuButtonPressed = false;
    this->sessionStarted = false;

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

    // init menu
    QWidget *screen = ui->Screen;
    vector<string> menuOptions = this->device.getScreen()->getMenuOptions();
    this->menuLayout = new QVBoxLayout(screen);
    for (const string &option : menuOptions)
    {
        QLabel *optionLabel = new QLabel(QString::fromStdString(option), screen);
        optionLabel->setStyleSheet("border: 1px solid black;");
        optionLabel->hide();
        menuLayout->addWidget(optionLabel);
        menuLayout->setAlignment(optionLabel, Qt::AlignHCenter | Qt::AlignVCenter);
        menuOptionLabels.append(optionLabel);
    }
    screen->setLayout(menuLayout);


}

/**
  display menu when menu button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::menuButtonPressed()
{
    this->selectedMenuOption = 0;
    this->isMenuButtonPressed = true;

    ui->Graph->hide();

    menuOptionLabels.at(0)->show();
    menuOptionLabels.at(1)->show();
    menuOptionLabels.at(2)->show();

    ui->CoherenceLabel->hide();
    ui->LengthLabel->hide();
    ui->AchievementLabel->hide();


    QLabel *label = menuOptionLabels.at(0);
    label->setStyleSheet("border: 1px solid yellow;");
}

/**
  handle scenario selector button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::selectorButtonPressed()
{
    if (selectedMenuOption == 0 && isMenuButtonPressed)
    {
        ui->Graph->show();
        menuOptionLabels.at(0)->hide();
        menuOptionLabels.at(1)->hide();
        menuOptionLabels.at(2)->hide();


        if (!sessionStarted)
            startSession();

    }
}

/**
  start session
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::startSession()
{

}

/**
  handle scenario where down arrow is pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::downArrowPressed()
{
    if (selectedMenuOption != 2 && isMenuButtonPressed)
    {
        QLabel *label = menuOptionLabels.at(selectedMenuOption);
        label->setStyleSheet("border: 1px solid black;");
        this->selectedMenuOption++;
        label = menuOptionLabels.at(selectedMenuOption);
        label->setStyleSheet("border: 1px solid yellow;");
    }
}

/**
  handle scenario where up arrow is pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::upArrowPressed()
{
    if (selectedMenuOption !=0 && isMenuButtonPressed)
    {
        QLabel *label = menuOptionLabels.at(selectedMenuOption);
        label->setStyleSheet("border: 1px solid black;");
        this->selectedMenuOption--;
        label = menuOptionLabels.at(selectedMenuOption);
        label->setStyleSheet("border: 1px solid yellow;");
    }
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

        QLabel *sensorOffMessage = new QLabel("SENSOR OFF", ui->Screen);
        QVBoxLayout *layout = new QVBoxLayout(ui->Screen);
        layout->addWidget(sensorOffMessage);
        layout->setAlignment(sensorOffMessage, Qt::AlignHCenter | Qt::AlignTop);
        ui->Screen->setLayout(layout);
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
//    qInfo() << "Testing battery: set to 15%";
//    this->device.getBattery()->updateBatteryLevel(15);


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

