#include "ServoController.h"
#include <algorithm>
#include <iomanip>

ServoController::ServoController() : systemMode(SystemState::IDLE), systemSafetyLock(false) {
    registry.reserve(MAX_SERVOS);
}

ServoController::~ServoController() {
    registry.clear();
}

int ServoController::clampAngle(int angle) {
    return std::max(MIN_ANGLE, std::min(angle, MAX_ANGLE));
}

bool ServoController::attachServo(int servoId, int initialAngle) {
    if (registry.size() >= MAX_SERVOS) {
        std::cout << "[ERROR] Cannot attach Servo " << servoId << ". Hardware channel capacity reached.\n";
        return false;
    }
    
    // Ensure unique ID mapping
    for (const auto& s : registry) {
        if (s.id == servoId) return false;
    }

    ServoMotor newServo = {
        servoId,
        clampAngle(initialAngle),
        clampAngle(initialAngle),
        5, // Default movement rate step
        true
    };

    registry.push_back(newServo);
    std::cout << "[SUCCESS] Servo ID " << servoId << " initialized at " << initialAngle << "°.\n";
    return true;
}

bool ServoController::detachServo(int servoId) {
    auto it = std::remove_if(registry.begin(), registry.end(), 
        [servoId](const ServoMotor& s) { return s.id == servoId; });
    
    if (it != registry.end()) {
        registry.erase(it, registry.end());
        std::cout << "[INFO] Servo ID " << servoId << " successfully detached.\n";
        return true;
    }
    return false;
}

void ServoController::updateTargetAngle(int servoId, int target) {
    if (systemSafetyLock) {
        std::cout << "[WARNING] Override blocked. Safety emergency lock active.\n";
        return;
    }

    for (auto& s : registry) {
        if (s.id == servoId) {
            s.targetAngle = clampAngle(target);
            systemMode = SystemState::MANUAL_CONTROL;
            return;
        }
    }
    std::cout << "[FAIL] Servo ID " << servoId << " not registered.\n";
}

void ServoController::processStep() {
    bool movementsPending = false;
    
    for (auto& s : registry) {
        if (s.currentAngle != s.targetAngle) {
            movementsPending = true;
            if (s.currentAngle < s.targetAngle) {
                s.currentAngle = std::min(s.targetAngle, s.currentAngle + s.speed);
            } else {
                s.currentAngle = std::max(s.targetAngle, s.currentAngle - s.speed);
            }
        }
    }

    if (!movementsPending && systemMode == SystemState::MANUAL_CONTROL) {
        systemMode = SystemState::IDLE;
    }
}

void ServoController::executeSweep(int servoId, int start, int end, int stepDelay) {
    std::cout << "[EXECUTE] Initiating hardware sweep profiling for Servo ID: " << servoId << "\n";
    systemMode = SystemState::AUTOMATIC_SWEEP;
    
    start = clampAngle(start);
    end = clampAngle(end);
    
    updateTargetAngle(servoId, start);
    while (registry[0].currentAngle != start) { processStep(); } // Simulate execution loops
    
    updateTargetAngle(servoId, end);
    while (registry[0].currentAngle != end) { processStep(); }
}

void ServoController::setSystemState(SystemState state) {
    systemMode = state;
}

void ServoController::toggleSafetyLock(bool active) {
    systemSafetyLock = active;
    std::cout << "[SYSTEM] Safety Lock state modified to: " << (active ? "ACTIVE" : "INACTIVE") << "\n";
}

void ServoController::printSystemStatus() const {
    std::string modeStr;
    switch (systemMode) {
        case SystemState::IDLE: modeStr = "IDLE"; break;
        case SystemState::MANUAL_CONTROL: modeStr = "MANUAL CONTROLS"; break;
        case SystemState::AUTOMATIC_SWEEP: modeStr = "AUTO SWEEP ROUTINE"; break;
        case SystemState::SEQUENCE_RUNNING: modeStr = "MACRO SEQUENCE RUNNING"; break;
    }

    std::cout << "\n=============================================\n";
    std::cout << " SYSTEM HARDWARE DASHBOARD | Mode: " << modeStr << "\n";
    std::cout << "=============================================\n";
    std::cout << " ID  | Current Pos | Target Pos  | Attached \n";
    std::cout << "-----|-------------|-------------|-----------\n";
    
    for (const auto& s : registry) {
        std::cout << " " << std::setw(3) << s.id << " | "
                  << std::setw(9) << s.currentAngle << "° | "
                  << std::setw(9) << s.targetAngle << "° | "
                  << (s.isAttached ? "YES" : "NO") << "\n";
    }
    std::cout << "=============================================\n";
}
