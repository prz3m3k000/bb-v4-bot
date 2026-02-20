#include "StopWatch.h"

uint32_t StopWatch::elapsed() const {
    return (micros() - previousMicros) / 1000;
}

uint32_t StopWatch::delta() {
    const uint32_t now = micros();
    const uint32_t delta = now - previousMicros;
    previousMicros = now;
    return delta / 1000;
}

void StopWatch::reset() {
    previousMicros = micros();
}
