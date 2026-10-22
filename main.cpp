#include "ServoController.h"

int main() {
    // Instantiate our multi-channel robotics core framework
    ServoController telemetryHub;

    std::cout << "[INIT] Initializing multi-axis subsystem runtime test pipeline...\n";

    // 1. Rig dynamic hardware attachments
    telemetryHub.attachServo(10, 0);   // Base Joint
    telemetryHub.attachServo(11, 90);  // Elbow Joint
    telemetryHub.attachServo(12, 180); // End Effector Wrist

    // Print initial state machine baseline
    telemetryHub.printSystemStatus();

    // 2. Queue movement updates (Manual Control Mode)
    std::cout << "\n[TEST] Pushing target orientation data changes downstream...\n";
    telemetryHub.updateTargetAngle(10, 45);
    telemetryHub.updateTargetAngle(11, 120);

    // Run structural iteration cycles to simulate clock ticks moving the motors smoothly
    for (int tick = 0; tick < 10; ++tick) {
        telemetryHub.processStep();
    }
    
    telemetryHub.printSystemStatus();

    // 3. Automation Subsystem Sweep Test
    telemetryHub.executeSweep(10, 0, 90, 15);
    telemetryHub.printSystemStatus();

    // 4. Test Safety Interrupt Protocols
    std::cout << "\n[TEST] Triggering emergency safety routines...\n";
    telemetryHub.toggleSafetyLock(true);
    telemetryHub.updateTargetAngle(12, 45); // Should fail due to safety lock active

    std::cout << "\n[CLEANUP] Detaching device nodes. Terminating processes.\n";
    telemetryHub.detachServo(10);
    
    return 0;
}
