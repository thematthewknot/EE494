/*
Pin Setup
LED pin 12, output LOW=ON HIGH=OFF 
GPS pin 5, input serial 4800
LogOmatic pin 6, output serial 9600
Autozero pin 11
Servo 1 pin 9, output PWM
Servo 2 pin 10, output PWM
Temp, pin A4, A5 i2C
Gyro, pin A0 
*/




#include <NewSoftSerial.h>

#include <SoftwareSerial.h>
#include <string.h>
#include "Wire.h"


void Temprature();
void GPS();
void Gyro();
void PID();
void Log(String);

//varibles needed for temprature reading.
int i,x;
byte res;
byte msb;
byte lsb;
int val;
//end of varibles needed for temprature reading.

void setup() 
{  
	Serial.begin(9600);
	Wire.begin(); i = 0; 
}

void loop()
{
	temprature();
	GPS();
	Gyro();
	PID();
	Log();
	
}

void temprature()//Read temp. data.
{  
	res = Wire.requestFrom(72,2); 
	if (res == 2) 
	{
		msb = Wire.receive(); /* Whole degrees */ 
		lsb = Wire.receive(); /* Fractional degrees */ 
		val = ((msb) << 4);  // MSB
		val |= (lsb >> 4);   // LSB
		Serial.print(val*0.0625);/* convert to Deg C(>0!)*/
		delay(1000); 
	}
}


void GPS()//Read GPS data.
{
}

void Gyro()//Read Gyro data
{

  x = analogRead(0);       // read analog input pin 0
  Serial.print("rotational rates are x, y: ");
  Serial.print(x, DEC);    // print the rotational rate in the X axis
  Serial.println(" ");       // prints a space between the numbers
  

}
void PID()//PID control.
{
}
void Log(String Logme)//print string passed to function to datalogger.
{
	Serial.print(logme);
}
