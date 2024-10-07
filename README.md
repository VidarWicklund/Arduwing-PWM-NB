This project aims at letting the user mix standard PWM receiver outputs for a flying wing. It can often be done on the transmitter, but on older/cheaper ones this might be difficult.

It takes the input from Throttle, Pitch and Roll and mixes the Pitch and Roll channels to left and right flap. Throttle is passed through but proportional, meaning that even if the receiver
output is limited range the entire ESC range is used.

In the future there will be functions to add stabilizing using an accelerometer/gyro.

Hardware:
- Arduino nano (should work on other Arduinos as well, also ESP32 using arduino framework with minor adjustments
- MPU-6050 (coming soon) for self stabilizing
- PWM rx/tx combo
- Flying wing platform with electronics (servos, esc, motor, battery)
