#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Button: public QObject{
    Q_OBJECT;

private:
    bool isPressed;

public:
    Button();
    void setButtonState(bool);
    bool getbuttonState();
};

#endif // BUTTON_H
