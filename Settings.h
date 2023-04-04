#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Settings {

private:
    int challengeLevel;
    int breathPacer;

public:
    Settings();
    void updateChallengLevel(int);
    void updateBreathPacer(int);

public slots:
    void handleUpdatedInterval(int);



};

#endif // SETTINGS_H
