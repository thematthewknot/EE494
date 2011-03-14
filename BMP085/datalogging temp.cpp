#include <SoftwareSerial.h>
#include <string.h>
#include "Wire.h"

#define rxPin 4 
#define txPin 5 

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

int i;
byte res;
byte msb;
byte lsb;
int val;

void setup() 
{ 


	pinMode(txPin, OUTPUT);
	mySerial.begin(9600);      
    Serial.begin(9600);
    Wire.begin(); i = 0; 
  	
}  

 void loop() 
 { 
   string temp;    
  res = Wire.requestFrom(72,2); 
  if (res == 2) 
  { 
    msb = Wire.receive(); /* Whole degrees */ 
    lsb = Wire.receive(); /* Fractional degrees */ 

 
  val = ((msb) << 4);  // MSB
  val |= (lsb >> 4);   // LSB
    
 Serial.print(val*0.0625);/* convert to Deg C(>0!)*/
  
  Serial.print("\n"); 

 mySerial.print("temp is= "); 
 mySerial.print(val);/* convert to Deg C(>0!)*/
  //mySerial.print(val);/* convert to Deg C(>0!)*/
 mySerial.print("\n"); 	
			delay(1000); 

				}
				  
			
			                      // wait for half a second 
			  
		 
	
} 
