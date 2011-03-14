#include <Servo.h> 
/* Reset the servo to it's zero point so you can align the horn properly.
   Wire the servo as follow:
      Servo Red --> Arduino +5V
      Servo White or Orange --> Arduino Digital Pin 7
      Servo Black or Brown --> Ground (GND).
      
  Compile and upload to the Arduino.  Servo will sweep through 180 degrees than reset to zero.
   
 */
 
// Pin definitions
const int servoPin = 7;             // Servo that we're controling. 
// Variables.
Servo gumballServo;                 // the servo that dispenses a gumball


void setup() {
 
  gumballServo.attach(servoPin);    // init the servo.
  gumballServo.write(60); 
}

void loop() {
  // Do nothing here.
}


