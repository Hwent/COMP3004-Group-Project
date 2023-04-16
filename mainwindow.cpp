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
    this->setting = new Settings();

    connect(this->device.getSensor(), SIGNAL(sensorStateChanged()), this, SLOT(handleSensorStateChange()));
    connect(this->device.getBattery(), SIGNAL(batteryLevelUpdated()), this, SLOT(handleBatteryChange()));
    connect(ui->MenuButton, SIGNAL (released()), this, SLOT (menuButtonPressed()));
    connect(ui->DownButton, SIGNAL (released()), this, SLOT (downArrowPressed()));
    connect(ui->UpButton, SIGNAL (released()), this, SLOT (upArrowPressed()));
    connect(ui->SelectButton, SIGNAL (released()), this, SLOT (selectorButtonPressed()));
    connect(ui->backButton, SIGNAL (released()), this, SLOT (backButtonPressed()));
    connect(ui->test, SIGNAL(released()), this, SLOT (test()));
    connect(this->setting, SIGNAL(updateSettingsUI()), this, SLOT(handleUpdateSettings()));

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
    this->history=new History();
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
    QListWidget *mainMenuList = new QListWidget(stackedWidget);
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
        historyList = new QListWidget(stackedWidget);
        stackedWidget->addWidget(historyList);
        // Creat History item menu
        historyitemList = new QListWidget(historyList);
        stackedWidget->addWidget(historyitemList);
        // Add the QStackedWidget to the main layout
        menuLayout->setContentsMargins(20, 20, 20, 20);
        menuLayout->addWidget(stackedWidget);
        screen->setLayout(menuLayout);

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
    // Get the current index of the stacked widget
        int currentIndex = stackedWidget->currentIndex();
        if (currentIndex == 0) {
            ui->backButton->setEnabled(false);
        }else if(currentIndex==4){
            stackedWidget->setCurrentIndex(currentIndex - 1);
        } else{
            stackedWidget->setCurrentIndex(0);
        }

    // Removes settings connections if necessary
    if(setBreathPacer[0] != nullptr)
    {
        disconnect(setBreathPacer[0]);
        disconnect(setBreathPacer[1]);
    }
}

/**
  handle scenario selector button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::selectorButtonPressed()
{
    QListWidget *item = qobject_cast<QListWidget*>(stackedWidget->currentWidget());
        if (!item->count()) {
            // List is empty, do nothing
            return;
        }
    // Get the selected menu option
        QString option = item->currentItem()->text();


        // Switch to the correct screen based on the selected option
        if (option == "Start/End Session") {
            stackedWidget->setCurrentIndex(1); // Switch to the session screen
        } else if (option == "Settings") {
            stackedWidget->setCurrentIndex(2); // Switch to the settings screen
            handleUpdateSettings(); // set the settings widget
            setBreathPacer[0] = connect(ui->RightButton, SIGNAL(released()), this->setting, SLOT(increaseBP()));
            setBreathPacer[1] = connect(ui->LeftButton, &QPushButton::released, this->setting, &Settings::decreaseBP);
        } else if (option == "History") {
            updateHistoryMenu();
            stackedWidget->setCurrentIndex(3); // Switch to the history screen
        } else if (option.contains("Session")){
            initHistoryitem();
            stackedWidget->setCurrentIndex(4);
        } else if(option=="delete"){
             clearHistoryitem();
        }
        // Enable the back button
        ui->backButton->setEnabled(true);
}
void MainWindow::initHistoryitem()
{
    // Clear the history item list widget
        historyitemList->clear();
        QString selectedSession = historyList->currentItem()->text();
        int sessionNumber = selectedSession.mid(7).toInt();
        // Add items from the history
        historyitemList->addItem((history->getHistory()[sessionNumber-1]).toString());
        historyitemList->addItem("delete");
        historyitemList->item(0)->setData(Qt::UserRole, sessionNumber-1);
        historyitemList->setCurrentRow(0);

}
void MainWindow::clearHistoryitem(){

    QListWidgetItem *item = historyitemList->item(0);
    int i = item->data(Qt::UserRole).toInt();
    menuButtonPressed();
    history->deleteItem(i);
}

void MainWindow::updateHistoryMenu(){
    //init/update history menu
    historyList->clear();

    int size=history->getHistory().size();
    if (size>0){
    for (int i = 0; i < size; i++) {
                QString itemText = "Session" + QString::number(i+1);
                historyList->addItem(itemText);
            }
    historyList->setCurrentRow(0);
    }
}

void MainWindow::handleUpdateSettings()
{
    QListWidget *item = qobject_cast<QListWidget*>(stackedWidget->currentWidget());

    item->clear();

    item->addItem(QString("Set breath pacer interval: %1 sec").arg(setting->getBreathPacer()));
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

