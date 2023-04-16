#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "Device.h"
#include "Settings.h"
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
    History *history;


    bool powerState;
    bool sensorLightOn;
    QVector<QLabel *> menuOptionLabels;
    int selectedMenuOption;
    bool isMenuButtonPressed;
    bool sessionStarted;
    QVBoxLayout *menuLayout;
    QStackedWidget *stackedWidget;
    QListWidget *historyList;
    QListWidget *historyitemList;
    QMetaObject::Connection setBreathPacer[2];




private slots:
    void test();
    void changePower();
    void menuButtonPressed();
    void downArrowPressed();
    void upArrowPressed();
    void selectorButtonPressed();
    void backButtonPressed();
    void startSession();
    //void logpressed();
    void handleSensorStateChange();
    void handleBatteryChange();
    void handleUpdateSettings();
    void initHistoryitem();
    void clearHistoryitem();
    void updateHistoryMenu();


};
#endif // MAINWINDOW_H
