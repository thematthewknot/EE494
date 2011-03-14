#include <NewSoftSerial.h>

#define rxPin 4 
#define txPin 5 

NewSoftSerial mySerial(rxPin, txPin);


void setup() 
{ 


	pinMode(txPin, OUTPUT);
	mySerial.begin(9600);      
  	
}  

 void loop() 
 { 
        
				
				mySerial.print("This is a test ");  
				mySerial.print("Should have a new line here...");
				mySerial.print("\n");    //new line char.
			
				mySerial.print("This is a test ");  
				mySerial.print("Should have a new line here...");
				mySerial.print("\n");
				
				mySerial.print("This is a test ");  
				mySerial.print("Should have a new line here...");
				mySerial.print("\n");
			delay(500);                       // wait for half a second 
			  
		 
	
} 
