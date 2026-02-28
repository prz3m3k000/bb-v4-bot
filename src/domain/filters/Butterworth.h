#pragma once

class Butterworth {
public:
    Butterworth(float cf0, float cf1, float cf2);
    float filter(float input);

private:
    float cf0, cf1, cf2;
    float v0 = 0, v1 = 0, v2 = 0;

};
