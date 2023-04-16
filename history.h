#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <SimulationData.h>
#include <QDateTime>

using namespace std;

class History: public QObject
{
       Q_OBJECT;
public:
    History();
    vector<SimulationData> getHistory();
    void deleteItem(int);
private:
    vector<SimulationData> historyList;


};

#endif // HISTORY_H
