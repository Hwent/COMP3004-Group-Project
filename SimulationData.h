#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <iostream>
#include <QString>
#include <QDebug>

using namespace std;

class SimulationData {

private:
    int coherenceScore;
    int challengeLevel;
    int breathPacer;
public:
    SimulationData(int,int,int);
    QString toString();

};

#endif // SIMULATIONDATA_H
