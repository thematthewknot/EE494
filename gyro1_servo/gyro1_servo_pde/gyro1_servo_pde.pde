/********************************************************
 * PID Simple Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/
#include <Servo.h>
#include <PID_Beta6.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output,angle;
Servo myservo1, myservo2; 
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1);
  
void setup()
{  
  myservo1.attach(9);
  myservo2.attach(10);
//  angle = 60;
  //initialize the variables we're linked to
  Input = analogRead(0);
  Setpoint = 240;
Serial.begin(9600);
  //turn the PID on
  myPID.SetMode(AUTO);
    myPID.SetOutputLimits(0,120);

  
  myservo1.write(60);
  myservo2.write(60);
}

void loop()
{
  Input = analogRead(0);
  myPID.Compute();
  //analogWrite(3,Output);
  Serial.print(analogRead(0));
   Serial.print("  ");
    Serial.print(Output);
  
 if(Input>=Setpoint)
 {Serial.print("more then");
    angle = 60 - Output;
   
 }
 if(Input < Setpoint)
 {
   Serial.print("less then");
    angle = Output + 60 ;

 }
     myservo1.write(angle);
  myservo2.write(angle);
     Serial.print("  ");
    Serial.println(angle);
  delay(10);
}


