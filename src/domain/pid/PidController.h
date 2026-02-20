#pragma once

class PidController {
public:
    float compute(float input, float setPoint, float dt);

    float compute2(float input, float inputDerivative, float setPoint, float dt);

    void setCoefficients(float kP, float kI, float kD);

    void setOutputRange(float minOutput, float maxOutput);

    void reset();

private:
    float alpha = 0.8;
    float kP = 0;
    float kI = 0;
    float kD = 0;
    float integral = 0;
    float lastInput = 0;
    float lastError = 0;
    float lastDerivative = 0;
    float minOutput = 0;
    float maxOutput = 0;
};
