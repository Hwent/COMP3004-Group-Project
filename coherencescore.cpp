#include "coherencescore.h"

CoherenceScore::CoherenceScore()
{

}

/**
 * @brief Returns a random coherence score, normalized to i
 * @param i
 * @param time
 * @return
 */
int CoherenceScore::calculateCoherence(int i, QDateTime time)
{
    qsrand(time.currentSecsSinceEpoch());
    int num = qrand() %100;

    if (num < 10)
        return 0;
    else if (num < 30)
        return 1;
    else if (num < 45)
        return 2;
    else if (num < 55)
        return 3;
    else if (num < 65)
        return 4;
    else
        return num %16;
}
