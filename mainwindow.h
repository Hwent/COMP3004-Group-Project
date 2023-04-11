#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "Device.h"

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

private slots:
    void test();
    void changePower();
    void menuButtonPressed();
    void downArrowPressed();
    void upArrowPressed();
    void selectorButtonPressed();
    void startSession();

    void handleSensorStateChange();
    void handleBatteryChange();
};
#endif // MAINWINDOW_H
