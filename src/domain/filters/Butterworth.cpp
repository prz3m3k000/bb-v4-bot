#include "Butterworth.h"

Butterworth::Butterworth(const float cf0, const float cf1, const float cf2) {
    this->cf0 = cf0;
    this->cf1 = cf1;
    this->cf2 = cf2;
}

float Butterworth::filter(const float input) {
    v0 = v1;
    v1 = v2;
    v2 = input * cf0 + cf1 * v0 + cf2 * v1;
    return v0 + v2 + 2 * v1;
}


