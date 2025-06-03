# IMU Calibration Workflow

QField can send vendor specific commands to external GNSS receivers to calibrate integrated IMU sensors.

1. Connect the receiver via Bluetooth.
2. Open the IMU calibration panel from the application UI.
3. Enter the antenna to IMU offset parameters and trigger calibration.
4. Optionally enable slant measurements or reset the INS module.

Calibration commands are issued over the Bluetooth connection using vendor proprietary messages. A simulator is available in the test suite to verify the communication.
