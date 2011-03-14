#include "Wire.h"
int i;
byte res;
byte msb;
byte lsb;
int val;
void setup() 
{ 
  Serial.begin(9600);
  Wire.begin(); i = 0; 
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
    Serial.print(val*0.0625);/* convert to Deg C(>0!)*/
	delay(1000); 
  }
}
