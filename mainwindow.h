#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "Device.h"
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


    bool powerState;
    bool sensorLightOn;
    QVector<QLabel *> menuOptionLabels;
    int selectedMenuOption;
    bool isMenuButtonPressed;
    bool sessionStarted;
    QVBoxLayout *menuLayout;
    QStackedWidget *stackedWidget;
    QListWidget *mainMenuList;



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

};
#endif // MAINWINDOW_H
