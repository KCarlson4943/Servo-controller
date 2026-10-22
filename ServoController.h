#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <iostream>
#include <vector>
#include <string>

// Maximum physical constraints for RC Servo motors
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;
const int MAX_SERVOS = 4;

enum class SystemState {
    IDLE,
    MANUAL_CONTROL,
    AUTOMATIC_SWEEP,
    SEQUENCE_RUNNING
};

struct ServoMotor {
    int id;
    int currentAngle;
    int targetAngle;
    int speed; // Degrees per cycle step
    bool isAttached;
};

class ServoController {
private:
    std::vector<ServoMotor> registry;
    SystemState systemMode;
    bool systemSafetyLock;

    // Internal safety and validation utility
    int clampAngle(int angle);

public:
    // Constructor & Destructor
    ServoController();
    ~ServoController();

    // Core Hardware Management API
    bool attachServo(int servoId, int initialAngle = 90);
    bool detachServo(int servoId);
    
    // Movement APIs
    void updateTargetAngle(int servoId, int target);
    void processStep(); // Executes trajectory interpolation frame
    void executeSweep(int servoId, int start, int end, int stepDelay);
    
    // System Configurations
    void setSystemState(SystemState state);
    void toggleSafetyLock(bool active);
    void printSystemStatus() const;
};

#endif // SERVO_CONTROLLER_H
