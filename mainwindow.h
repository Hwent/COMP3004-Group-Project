#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void test();
    void changeBatteryLevel(double);
    void changePower();

    void handleSensorStateChange();
    void handleBatteryChange();
};
#endif // MAINWINDOW_H
