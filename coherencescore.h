#ifndef COHERENCESCORE_H
#define COHERENCESCORE_H

#include <QObject>
#include <QDateTime>

class CoherenceScore : public QObject
{
    Q_OBJECT
public:
    CoherenceScore();

    int calculateCoherence(int, QDateTime);

private:
    QDateTime time;
    int value;
};

#endif // COHERENCESCORE_H
