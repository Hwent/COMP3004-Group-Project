#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "Device.h"
#include "Settings.h"
#include <QTimer>
#include <QStackedWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void initGui();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Device device;
    Settings *setting;

    bool powerState;
    bool sensorLightOn;
    QVector<QLabel *> menuOptionLabels;
    int selectedMenuOption;
    bool isMenuButtonPressed;
    bool sessionStarted;
    QVBoxLayout *menuLayout;
    QVector<double> hrvData;
    QVector<double> timeData;
    QVector<qreal> coherenceData;
    QStackedWidget *stackedWidget;
    QListWidget *mainMenuList;
    QMetaObject::Connection setBreathPacer[2];
    QListWidget *historyList;
    QWidget *logHistoryMetrics;
    QLabel *logHistoryMetricsLabel;
    QListWidget *logList;

    // update/calculate metrics
    QTimer timer;
    int sessionLength;
    double sum;    // used for calculating achievement score
    int coherenceIndex;      // used for getting coherence score
    double achievementScore;
    QVector<double> previousCoherenceScores;
    int prevCoherenceLevel;
    QVector<int> timeInCoherencelevel;  // used to calculate percentage of time in each coherence level

private slots:
    void sensorScenario();
    void lowBatteryScenario();

    void changePower();
    void menuButtonPressed();
    void downArrowPressed();
    void upArrowPressed();
    void selectorButtonPressed();
    void startSession();
    void endSession();
    void plotGraph();
    void updateSessionMetrics();
    void backButtonPressed();

    void handleSensorStateChange();
    void handleBatteryChange();
    void handleUpdateSettings();
};
#endif // MAINWINDOW_H
