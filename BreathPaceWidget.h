#ifndef BREATHPACEWIDGET_H
#define BREATHPACEWIDGET_H

#include <iostream>
#include <string>
#include <QDebug>
#include <QWidget>

using namespace std;

class BreathPaceWidget {

private:
    int interval;
    QString visualIndicator;

public:
    BreathPaceWidget();
    void updateInterval(int);

signals:
    void intervalUpdated(int);

};

#endif // BREATHPACEWIDGET_H
