#pragma once

#include <Arduino.h>

class StopWatch {
public:
    uint32_t elapsed() const;
    uint32_t delta();
    void reset();

    private:
        uint32_t previousMicros = 0;
};
