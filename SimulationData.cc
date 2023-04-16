#include "SimulationData.h"

SimulationData::SimulationData(int coherenceScore, int challengeLevel, int breathPacer){
    this->coherenceScore=coherenceScore;
    this->challengeLevel=challengeLevel;
    this->breathPacer=breathPacer;
    currentDateTime = QDateTime::currentDateTime();
}

QString SimulationData::toString() {
    QString newString =
            "   coherenceScore: " + QString::number(coherenceScore) + "\n"
            + "   challengeLevel: " + QString::number(challengeLevel) + "\n"
            + "   breathPacer: " + QString::number(breathPacer)+ "\n"
            + "Date: " + currentDateTime.toString(Qt::ISODate);
    return newString;
}
