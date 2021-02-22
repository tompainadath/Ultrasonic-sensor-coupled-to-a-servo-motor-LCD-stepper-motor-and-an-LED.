# Ultrasonic-sensor-coupled-to-a-servo-motor-LCD-stepper-motor-and-an-LED.
# Project Proposal:
Specific tasks where to turn both the servo and stepper motor in clockwise direction when an object is 20 inches or greater from the ping sensor. If the object is at a distance between 5 and 20 inches, blink an LED (or all of them). The blinking speed of the LED is determined by the distance and should blink slowly starting at 20 inches and incrementally “speed up” as you reach 5 inches. The servo and stepper motors should not turn on in this distance range. And If the object is 5 inches or closer towards the ping sensor, the LED should light up solid and both the servo and stepper motor should rotate counter clockwise with no delay and should display in inches on the LCD the distance. The servo should turn off when the object is further than 10 inches away. The LED(s) should return back to a blink if it is between 5 inches and 20 inches and should turn off for distances greater than 20 inches.
# Main Operational Method:
Two interrupt service routines were used in the code for this project. One was Timer2 overflow interrupt and the other was pin change interrupt 3. The Timer2 overflow interrupt was triggered by the output compare flag and the pin change interrupt 3 service routine was triggered by the change in pin change interrupt flag. 
# Flow Chart:
 
Figure 1. Flow chart


 
# Results
The system of devices worked as a unit to produce the expected the result. When an object was placed beyond the 20 inches mark from the Ping sensor, both the servo motor and stepper motor turned in the clockwise direction and as the object was moved to between the distance the distance of 20 and 5 inches, the PORTA LEDs started blinking with increasing speed. Meanwhile, the Servo motor and stepper motor stopped turning. And when the object was moved inside the 5 inches mark, the Servo motor and the stepper motor started turning counter clockwise. Meanwhile, the LEDs were light up solid. 
