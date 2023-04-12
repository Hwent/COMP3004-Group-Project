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
    connect(ui->backButton, SIGNAL (released()), this, SLOT (backButtonPressed()));
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
    stackedWidget = new QStackedWidget(screen);

    //vector<string> menuOptions = this->device.getScreen()->getMenuOptions();
    //create main menu
    this->menuLayout = new QVBoxLayout(screen);
    mainMenuList = new QListWidget(stackedWidget);
    mainMenuList->addItem("Start/End Session");
    mainMenuList->addItem("Settings");
    mainMenuList->addItem("History");
    mainMenuList->setCurrentRow(0);
    stackedWidget->addWidget(mainMenuList);

        // Create Session menu
        QListWidget *sessionList = new QListWidget(stackedWidget);
        stackedWidget->addWidget(sessionList);
        // Create Settings menu
        QListWidget *settingsList = new QListWidget(stackedWidget);
        stackedWidget->addWidget(settingsList);
        // Create History menu
        QListWidget *historyList = new QListWidget();
        stackedWidget->addWidget(historyList);


        // Add the QStackedWidget to the main layout
        menuLayout->setContentsMargins(20, 20, 20, 20);
        menuLayout->addWidget(stackedWidget);
        screen->setLayout(menuLayout);


        //init log/history menu
        vector<SimulationData> datalist = this->device.getScreen()->getlog();
        QStringList history=QStringList();
        for (SimulationData data : datalist)
        {
        historyList->addItem(data.toString());
        }
        historyList->setCurrentRow(0);

}

/**
  display menu when menu button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::menuButtonPressed()
{
    ui->Graph->hide();
    ui->CoherenceLabel->hide();
    ui->LengthLabel->hide();
    ui->AchievementLabel->hide();

    stackedWidget->setCurrentIndex(0);
}
/**
  handle scenario back button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::backButtonPressed()
{
    // Switch back to the main menu screen
        stackedWidget->setCurrentIndex(0);


        // Disable the back button
        ui->backButton->setEnabled(false);
}

/**
  handle scenario selector button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::selectorButtonPressed()
{
    // Get the selected menu option
        QListWidgetItem *item = mainMenuList->currentItem();
        QString option = item->text();

        // Switch to the correct screen based on the selected option
        if (option == "Start/End Session") {
            stackedWidget->setCurrentIndex(1); // Switch to the session screen
        } else if (option == "Settings") {
            stackedWidget->setCurrentIndex(2); // Switch to the settings screen
        } else if (option == "History") {
            stackedWidget->setCurrentIndex(3); // Switch to the history screen
        }


        // Enable the back button
        ui->backButton->setEnabled(true);
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
    QListWidget *listWidget = qobject_cast<QListWidget*>(stackedWidget->currentWidget());
        //qInfo() <<"stackedWidget position:" <<stackedWidget->currentIndex()<<listWidget;
        if (listWidget) { // make sure the current page is a QListWidget
            int currentIndex = listWidget->currentRow();
            if (currentIndex < listWidget->count() - 1) {
                listWidget->setCurrentRow(currentIndex + 1);
            }

        }

}

/**
  handle scenario where up arrow is pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::upArrowPressed()
{
    QListWidget *listWidget = qobject_cast<QListWidget*>(stackedWidget->currentWidget());

        if (listWidget) { // make sure the current page is a QListWidget
            int currentIndex = listWidget->currentRow();
            if (currentIndex > 0) {
                listWidget->setCurrentRow(currentIndex - 1);
            }
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

