#ifndef BREATHPACEWIDGET_H
#define BREATHPACEWIDGET_H

#include <iostream>
#include <string>
#include <QDebug>
#include <QWidget>
#include <QLabel>

using namespace std;

class BreathPaceWidget : public QWidget {

Q_OBJECT

private:
    int interval;
    QLabel *breathPaceLabel;

public:
    explicit BreathPaceWidget(QWidget *parent = nullptr);
    void updateInterval(int);
    void updateBreathPaceLabel();

signals:
    void intervalUpdated(int);

};

#endif // BREATHPACEWIDGET_H
