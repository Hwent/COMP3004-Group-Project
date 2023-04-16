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

    connect(&timer, &QTimer::timeout, this, &MainWindow::updateSessionMetrics);
    connect(this->device.getSensor(), SIGNAL(sensorStateChanged()), this, SLOT(handleSensorStateChange()));
    connect(this->device.getBattery(), SIGNAL(batteryLevelUpdated()), this, SLOT(handleBatteryChange()));
    connect(ui->MenuButton, SIGNAL (released()), this, SLOT (menuButtonPressed()));
    connect(ui->DownButton, SIGNAL (released()), this, SLOT (downArrowPressed()));
    connect(ui->UpButton, SIGNAL (released()), this, SLOT (upArrowPressed()));
    connect(ui->SelectButton, SIGNAL (released()), this, SLOT (selectorButtonPressed()));
    connect(ui->backButton, SIGNAL (released()), this, SLOT (backButtonPressed()));
    connect(this->setting, SIGNAL(updateSettingsUI()), this, SLOT(handleUpdateSettings()));

    connect(ui->SensorScenario, SIGNAL(released()), this, SLOT (sensorScenario()));
    connect(ui->LowBatteryScenario, SIGNAL(released()), this, SLOT (lowBatteryScenario()));

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
    this->sessionLength = 0;
    this->sum = 0;
    this->coherenceIndex = 0;
    this->achievementScore = 0;
    this->previousCoherenceScores = {};
    this->prevCoherenceLevel = 0;
    this->timeInCoherencelevel = {0, 0, 0};

    this->setting = new Settings();

    // coherence lights default
    ui->RedLight->setStyleSheet("QLabel {background-color: rgb(207, 124, 136);}");
    ui->GreenLight->setStyleSheet("QLabel {background-color: lightgreen;}");
    ui->BlueLight->setStyleSheet("QLabel {background-color: lightblue;}");

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

    this->menuLayout = new QVBoxLayout(screen);
    stackedWidget = new QStackedWidget(screen);

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
    historyList = new QListWidget(stackedWidget);
    stackedWidget->addWidget(historyList);
    // create logs selection
    logList = new QListWidget(stackedWidget);
    stackedWidget->addWidget(logList);

    // Add the QStackedWidget to the main layout
    menuLayout->setContentsMargins(20, 20, 20, 20);
    menuLayout->addWidget(stackedWidget);
    screen->setLayout(menuLayout);

    //init log/history menu
    QMap<QDateTime, QVector<double>> logHistory = device.getScreen()->getLog();
    QStringList history=QStringList();
    QList<QDateTime> dateTimes = logHistory.keys();

    for (const QDateTime &dateTime : dateTimes)
        historyList->addItem(dateTime.toString("yyyy-MM-dd hh:mm:ss"));

    historyList->setCurrentRow(0);

    // sample data
    hrvData = {800, 820, 790, 805, 830, 810, 800, 795, 815, 790, 810, 800, 820, 780, 800, 820, 795,
                805, 810, 790, 800, 830, 810, 790, 800, 820, 780, 800, 820, 795, 805, 810, 790, 800,
                820, 790, 805, 830, 810, 800, 795, 815, 790, 810, 800, 820, 780, 800, 820, 795, 805,
                810, 790, 815, 790, 810, 800, 820, 780, 800};

    for (int i = 1; i <= 60; i++)
        timeData.append(i);

//    coherenceData = {0.3, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 0.2, 2.4, 6.5, 16, 2.7};
     coherenceData = {0.3, 0.5, 1.5, 2.0, 3.0, 0.2, 2.4, 6.5, 16};

    // init graph
     QCustomPlot *customPlot = ui->Graph;
     customPlot->xAxis2->setVisible(false);
     customPlot->xAxis2->setTickLabels(false);
     customPlot->yAxis2->setVisible(false);
     customPlot->yAxis2->setTickLabels(false);
     customPlot->xAxis->setTickLabels(false);
     customPlot->yAxis->setTickLabels(false);
     customPlot->xAxis->setLabel("Time");
     customPlot->xAxis->setLabelFont(QFont("Arial", 8));
     customPlot->yAxis->setLabel("HRV");
     customPlot->yAxis->setLabelFont(QFont("Arial", 8));

}

/**
  update the session metrics every 5 seconds
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::updateSessionMetrics()
{
    sessionLength += 5;
    if (sessionLength != 0 && coherenceIndex < coherenceData.size())
    {
        ui->RedLight->setStyleSheet("QLabel {background-color: rgb(207, 124, 136);}");
        ui->GreenLight->setStyleSheet("QLabel {background-color: lightgreen;}");
        ui->BlueLight->setStyleSheet("QLabel {background-color: lightblue;}");

        // update/calculate metrics
        ui->CoherenceVal->setNum(coherenceData[coherenceIndex]);
        ui->LengthVal->setNum(sessionLength);
        previousCoherenceScores.append(coherenceData[coherenceIndex]);

        for (int k = 0; k < previousCoherenceScores.size(); k++)
            sum += previousCoherenceScores[k];

        achievementScore = sum/(previousCoherenceScores.size()+1);
        double temp = round(achievementScore*100)/100;
        ui->AchievementVal->setNum(temp);

        // update coherence lights
        int coherenceLevel = 0;
        if (coherenceData[coherenceIndex] < 0.6)
        {
            ui->RedLight->setStyleSheet("QLabel {background-color: red;}");
            coherenceLevel = 1;
            timeInCoherencelevel[0] += 5;
        }
        else if (coherenceData[coherenceIndex] > 0.5 && coherenceData[coherenceIndex] < 3.1)
        {
            ui->BlueLight->setStyleSheet("QLabel {background-color: blue;}");
            coherenceLevel = 2;
            timeInCoherencelevel[1] += 5;
        }
        else
        {
            ui->GreenLight->setStyleSheet("QLabel {background-color: green;}");
            coherenceLevel = 3;
            timeInCoherencelevel[2] += 5;
        }

        if (coherenceLevel != prevCoherenceLevel)
        {
            //QApplication::beep();        Sound not enabled on VM so printing beep
            qInfo() << "New Coherence Level: Beep";
        }

        prevCoherenceLevel = coherenceLevel;
        coherenceIndex++;
    }
    else if (coherenceIndex >= coherenceData.size())
    {
        timer.stop();
        endSession();
    }
}

/**
  display menu when menu button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::menuButtonPressed()
{
    ui->SummaryView->hide();
    stackedWidget->show();
    ui->backButton->setEnabled(false);

    stackedWidget->setCurrentIndex(0);

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
        QDateTime dateTime;

        if (stackedWidget->currentIndex() == 3)
        {
            int historyItemIndex = historyList->currentRow();
            QMap<QDateTime, QVector<double>> logHistory = device.getScreen()->getLog();
            QList<QDateTime> dateTimes = logHistory.keys();

            // get data corresponding to DateTime
            for (int i = 0; i < dateTimes.size(); i++)
            {
                if (i == historyItemIndex)
                {

                    //stackedWidget->setCurrentIndex(4
                    dateTime = dateTimes.at(i);
                    double avgCoherence = logHistory.value(dateTime)[0];
                    double sessionLength = logHistory.value(dateTime)[1];
                    double achievementScore = logHistory.value(dateTime)[2];

                    QString labelText = QString("Log Summary:\nAverage Coherence: %1\nSession Length: %2\nAchievement Score: %3")
                            .arg(avgCoherence, 0, 'f', 2)
                            .arg(sessionLength)
                            .arg(achievementScore, 0, 'f', 2);
                    logList->addItem(labelText);
                    logList->addItem("Delete Log");
                    logList->setCurrentRow(0);
                    stackedWidget->setCurrentIndex(4);
                    return;
                }
            }
        }

        else if (stackedWidget->currentIndex() == 4)
        {
            QListWidgetItem *logItem = logList->currentItem();
            QString logOption = logItem->text();

            if (logOption == "Delete Log")
            {
                delete logList->takeItem(1);
                delete logList->takeItem(0);
                QMap<QDateTime, QVector<double>> logHistory = device.getScreen()->getLog();
                logHistory.remove(dateTime);

                for (int i = 0; i < historyList->count(); i++)
                {
                    if (historyList->item(i)->text() == dateTime.toString("yyyy-MM-dd hh:mm:ss"))
                        delete historyList->takeItem(i);
                }

                stackedWidget->setCurrentIndex(3);
                ui->Device->update();
                return;
            }
        }


        // Switch to the correct screen based on the selected option
        if (option == "Start/End Session")
        {
            stackedWidget->setCurrentIndex(1); // Switch to the session screen
            stackedWidget->hide();
            if (!sessionStarted)
                startSession();
        }
        else if (option == "Settings")
        {
            stackedWidget->setCurrentIndex(2); // Switch to the settings screen
            handleUpdateSettings(); // set the settings widget
            setBreathPacer[0] = connect(ui->RightButton, SIGNAL(released()), this->setting, SLOT(increaseBP()));
            setBreathPacer[1] = connect(ui->LeftButton, &QPushButton::released, this->setting, &Settings::decreaseBP);
        }
        else if (option == "History")
        {
            stackedWidget->setCurrentIndex(3); // Switch to the history screen
            //int historyItemIndex = historyList->currentRow();
        }


        // Enable the back button
        ui->backButton->setEnabled(true);
}

/**
  handle scenario back button pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::backButtonPressed()
{
    // Switch back to the main menu screen
    stackedWidget->show();
    stackedWidget->setCurrentIndex(0);

    logHistoryMetrics->hide();

    // Disable the back button
    ui->backButton->setEnabled(false);

    // Removes settings connections if necessary
    if(setBreathPacer[0] != nullptr)
    {
        disconnect(setBreathPacer[0]);
        disconnect(setBreathPacer[1]);
    }
}

/**
  start session
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::startSession()
{
    ui->CoherenceVal->setNum(0);
    ui->LengthVal->setNum(0);
    ui->AchievementVal->setNum(0);

    timer.start(5000);
    plotGraph();

}

/**
  end session, reset all variables and gui to default
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::endSession()
{
    // show summary view
    QWidget *summaryView = ui->SummaryView;
    summaryView->show();
    summaryView->setStyleSheet("QWidget {background-color: white}");

    double totalCoherence = 0;

    for (int i = 0; i < coherenceData.size(); i++)
    {
        totalCoherence += coherenceData[i];
    }
    double avgCoherence = static_cast<double>(totalCoherence)/coherenceData.size();

    QLabel *coherenceLevels = ui->CoherenceLevels;
    sessionLength -= 5;
    QString labelText = QString("Coherence Levels:\nLevel 1: %1%\nLevel 2: %2%\nLevel 3: %3%")
            .arg(static_cast<double>(timeInCoherencelevel[0])/sessionLength*100, 0, 'f', 2)
            .arg(static_cast<double>(timeInCoherencelevel[1])/sessionLength*100, 0, 'f', 2)
            .arg(static_cast<double>(timeInCoherencelevel[2])/sessionLength*100, 0, 'f', 2);
    coherenceLevels->setText(labelText);
    coherenceLevels->setStyleSheet("QLabel {font-size: 9px}");
    coherenceLevels->show();

    QLabel *metricsLabel = ui->MetricsLabel;
    labelText = QString("Metrics Summary:\nAverage Coherence: %1\nSession Length: %2\nAchievement Score: %3")
            .arg(avgCoherence, 0, 'f', 2)
            .arg(sessionLength)
            .arg(achievementScore, 0, 'f', 2);
    metricsLabel->setText(labelText);
    metricsLabel->setStyleSheet("QLabel {font-size: 9px}");
    metricsLabel->show();

    // save for history log before reset
    device.getScreen()->addLog(avgCoherence, static_cast<double>(sessionLength), achievementScore);
    QMap<QDateTime, QVector<double>> logHistory = device.getScreen()->getLog();
    QStringList history=QStringList();
    QList<QDateTime> dateTimes = logHistory.keys();
    qInfo() << logHistory;

    for (const QDateTime &dateTime : dateTimes)
        historyList->addItem(dateTime.toString());

    historyList->setCurrentRow(0);
    ui->Device->update();

    // reset variables
    sum = 0;
    achievementScore = 0;
    coherenceIndex = 0;
    previousCoherenceScores = {};
    prevCoherenceLevel = 0;
    sessionLength = 0;
    timeInCoherencelevel = {0, 0, 0};

    // reset gui
    ui->RedLight->setStyleSheet("QLabel {background-color: rgb(207, 124, 136);}");
    ui->GreenLight->setStyleSheet("QLabel {background-color: lightgreen;}");
    ui->BlueLight->setStyleSheet("QLabel {background-color: lightblue;}");
    ui->CoherenceVal->setNum(0);
    ui->LengthVal->setNum(0);
    ui->AchievementVal->setNum(0);
}

/**
  handle scenario where down arrow is pressed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::downArrowPressed()
{
    QListWidget *listWidget = qobject_cast<QListWidget*>(stackedWidget->currentWidget());

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

        QString sensorMessage = "SENSOR OFF";
    }
    else
    {
        sensor->setStyleSheet("QLabel {background-color: pink}");
        this->sensorLightOn = true;

    }

}

/**
  when settings are changed
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::handleUpdateSettings()
{
    QListWidget *item = qobject_cast<QListWidget*>(stackedWidget->currentWidget());

    item->clear();

    item->addItem(QString("Set breath pacer interval: %1 sec").arg(setting->getBreathPacer()));
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

    batteryBar->setValue(batteryLevel);
    if (batteryLevel <= 20)
    {
        batteryBar->setStyleSheet("QProgressBar::chunk {background-color: red}");
        QString lowBatteryMessage = "LOW BATTERY";
    }

}

/**
  Update graph using the hrv sample data
  @param {}
  @return {void} Returns nothing
*/
void MainWindow::plotGraph()
{
    QCustomPlot *customPlot = ui->Graph;
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));

    // make sin wave
    for (int i = 0; i < timeData.size(); i++)
        timeData[i] = i*0.1;

    for (int i = 0; i < hrvData.size(); i++)
        hrvData[i] = sin(timeData[i] * 2 * M_PI);

    customPlot->xAxis2->setVisible(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(false);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(false);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->xAxis->setLabel("Time");
    customPlot->xAxis->setLabelFont(QFont("Arial", 8));
    customPlot->yAxis->setLabel("HRV");
    customPlot->yAxis->setLabelFont(QFont("Arial", 8));
    customPlot->yAxis->setRange(770, 840);

    customPlot->graph(0)->setData(timeData, hrvData);
    customPlot->rescaleAxes();

    customPlot->replot();

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

void MainWindow::sensorScenario()
{
    // test heart monitor sensor
    qInfo() << "Testing sensor change";
    this->device.getSensor()->changeSensorState(!this->sensorLightOn);

}

void MainWindow::lowBatteryScenario()
{
    // test battery
    qInfo() << "Testing battery: set to 15%";
    this->device.getBattery()->updateBatteryLevel(15);
}
