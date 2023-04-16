#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Graph: public QObject
{
    Q_OBJECT;

public:
    Graph();
    void plotGraph(QVector<int>);
};

#endif // GRAPH_H
