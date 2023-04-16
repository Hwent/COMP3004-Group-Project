#include "SimulationData.h"

SimulationData::SimulationData()
{

}

//QString SimulationData::toString() {
////    QString newString =
////            "   Average Coherence: " + QString::number(coherenceScore) + "\n"
////            + "   Session Length: " + QString::number(challengeLevel) + "\n"
////            + "   Achievemnt Score: " + QString::number(breathPacer);
////    return newString;
//}

void SimulationData::addLog(double avgCoherence, double sessionLength, double achievementScore)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QVector<double> metrics = {avgCoherence, sessionLength, achievementScore};
    simulationLog.insert(currentDateTime, metrics);
}

QMap<QDateTime, QVector<double>> SimulationData::getLog() { return simulationLog; }
