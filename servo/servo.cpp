#include <Servo.h>

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

int pos = 0;    // variable to store the servo position

void setup()
{
  myservo.attach(8, 544, 2400);  // attaches the servo on pin 8 to the servo object
}


void loop()
{
                                // in steps of 1 degree
    myservo.write(60);              // tell servo to go to position in variable 'pos'
  
  
  // myservo.write(0);
} 
