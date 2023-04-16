#include "history.h"

History::History()
{
    this->historyList.push_back(SimulationData(0,0,0));
    this->historyList.push_back(SimulationData(1,1,1));

}

vector<SimulationData> History::getHistory(){ return this->historyList;}

void History::deleteItem(int i){
    historyList.erase(historyList.begin() + i);
}
