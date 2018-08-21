#include "FpsCounter.h"

FpsCounter::FpsCounter() {
    this->reset();
}

void FpsCounter::reset() {
    this->_timeTotal = 1.0f;
    this->_time1Sec  = 1.0f;
    this->_time3Sec  = 1.0f;
    this->_frameCntTotal = 0;
    this->_frameCnt1Sec  = 0;
    this->_frameCnt3Sec  = 0;

    this->_preFpsIn1Sec = 0.0;
    this->_preFpsIn3Sec = 0.0;
}

string FpsCounter::getFpsInfo() {
    char buf[256];
    sprintf(buf, "%.2f(1s) %.2f(3s) %.2f(total)", this->_preFpsIn1Sec, this->_preFpsIn3Sec, (this->_frameCntTotal / this->_timeTotal));
    return string(buf);
}

bool FpsCounter::update(double time) {
    bool bUpdate = false;
    this->_timeTotal += time;
    this->_time1Sec += time;
    this->_time3Sec += time;

    this->_frameCntTotal++;
    this->_frameCnt1Sec++;
    this->_frameCnt3Sec++;

    if (1.0f <= this->_time1Sec) {
        this->_preFpsIn1Sec = this->_frameCnt1Sec / this->_time1Sec ;
        this->_time1Sec = 0;
        this->_frameCnt1Sec = 0;
        bUpdate = true;
    }
    if (3.0f <= this->_time3Sec) {
        this->_preFpsIn3Sec = this->_frameCnt3Sec / this->_time3Sec ;
        this->_time3Sec = 0;
        this->_frameCnt3Sec = 0;
        bUpdate = true;
    }
    return bUpdate;
}