#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Settings: public QObject {

    Q_OBJECT

private:
    int challengeLevel;
    int breathPacer;

public:
    Settings();

    void updateChallengLevel(int);
    void updateBreathPacer(int);
    int getBreathPacer() {return breathPacer;}

signals:
    void updateSettingsUI();

public slots:
    void handleUpdatedInterval(int);
    void increaseBP();
    void decreaseBP();


};

#endif // SETTINGS_H
