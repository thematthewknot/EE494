/*
Pin Setup
LED pin 12, output LOW=ON HIGH=OFF 
GPS pin 5, input serial 4800
LogOmatic pin 6, output serial 9600
Autozero pin 11
Servo 1 pin 9, output PWM
Servo 2 pin 10, output PWM
Temp, pin A4, A5 i2C
Gyro, pin A0 input
*/

#include <NewSoftSerial.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "Wire.h"
#include <ctype.h>
#define rxPin 4 
#define txPin 6 

void Temprature();



NewSoftSerial mySerial(rxPin, txPin);


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
    pinMode(txPin, OUTPUT);
	mySerial.begin(9600);   
}

void loop()
{
		res = Wire.requestFrom(72,2); 
	if (res == 2) 
	{
		msb = Wire.receive(); /* Whole degrees */ 
		lsb = Wire.receive(); /* Fractional degrees */ 
		val = ((msb) << 4);  // MSB
		val |= (lsb >> 4);   // LSB
		Serial.println(val*0.0625);/* convert to Deg C(>0!)*/
		mySerial.println(val*0.0625, BIN);/* convert to Deg C(>0!)*/
                
		delay(1000); 
	}
	
	
	
	
	
}
