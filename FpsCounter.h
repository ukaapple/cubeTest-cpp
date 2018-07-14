#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <iostream>

using namespace std;

class FpsCounter {
public:
    FpsCounter();
    void reset();
    bool update(double time);
    string getFpsInfo();
private:
    double _timeTotal;
    double _time1Sec;
    double _time3Sec;
    long _frameCntTotal;
    long _frameCnt1Sec;
    long _frameCnt3Sec;

    double _preFpsIn1Sec;
    double _preFpsIn3Sec;
};

#endif
