#include "PidController.h"

float PidController::compute(const float input, const float setPoint, const float dt) {
    const float error = setPoint - input;
    const float pTerm = kP * error;

    const float potentialIntegral = integral + error * dt;

    float rawDerivative = 0.0;
    if (dt > 0.0) {
        rawDerivative = (input - lastInput) / dt;
    }
    const float filteredDerivative = alpha * rawDerivative + (1.0f - alpha) * lastDerivative;
    lastDerivative = filteredDerivative;

    const float dTerm = -kD * filteredDerivative;

    float output = pTerm + kI * potentialIntegral + dTerm;

    // limits on output
    if (output > maxOutput) {
        output = maxOutput;
        if (error < 0) {
            integral = potentialIntegral;
        }
    } else if (output < minOutput) {
        output = minOutput;
        if (error > 0) {
            integral = potentialIntegral;
        }
    } else {
        integral = potentialIntegral;
    }

    lastInput = input;
    return output;
}

float PidController::compute2(const float input, float inputDerivative, const float setPoint, const float dt) {
    const float error = setPoint - input;
    const float pTerm = kP * error;
    const float potentialIntegral = integral + error * dt;
    const float dTerm = -kD * inputDerivative;
    float output = pTerm + kI * potentialIntegral + dTerm;

    // limits on output
    if (output > maxOutput) {
        output = maxOutput;
        if (error < 0) {
            integral = potentialIntegral;
        }
    } else if (output < minOutput) {
        output = minOutput;
        if (error > 0) {
            integral = potentialIntegral;
        }
    } else {
        integral = potentialIntegral;
    }

    lastInput = input;
    return output;
}

void PidController::setCoefficients(const float kP, const float kI, const float kD) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

void PidController::setOutputRange(const float minOutput, const float maxOutput) {
    this->minOutput = minOutput;
    this->maxOutput = maxOutput;
}

void PidController::reset() {
    this->integral = 0;
    this->lastDerivative = 0;
    this->lastError = 0;
    this->lastInput = 0;
}
