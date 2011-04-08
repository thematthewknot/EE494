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
Example of how to write to datalogger: mySerial.println(val*0.0625, BIN);
Output data format:Time,control/notcontroll, Gyro pos, gyro rate, servo output, temp, gps(12 fields) 
* 
* 
* for timestamp use http://www.arduino.cc/en/Reference/Millis
* 
* 
* 
*/

#include <NewSoftSerial.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "Wire.h"
#include <ctype.h>
#include <Servo.h>
#include <PID_Beta6.h>

/*
void Temprature();
void GPS();
void Gyro();
void PIDCONTROL();
*/

//datalogger
#define rxPin 4 
#define txPin 6 
NewSoftSerial mySerial(rxPin, txPin);

//datalogger

//varibles needed for temprature reading.
int i,x;
byte res;
byte msb;
byte lsb;
int val;
//end of varibles needed for temprature reading.




//Gyro veribles
int loops=0; //configuration stage loops
int centerpoint = 0; // zero rotation value
int high = 0; // motionless calibration error
int low = 1023; // motionless calibration error
int sensor = 0; // analog gyro port
int variation = 0; // expected gyro jitter +- value
int change = 0; // ATD conversion change
float position = 0.0; // current cardinal position
unsigned long timetook = 0; //sample time taken
unsigned long lasttook = 0; //last sample time
 float printrateofchange, printposition;
 
//gyro



//pid

double Setpoint, Input, Output,angle;
Servo myservo1, myservo2; 
PID myPID(&Input, &Output, &Setpoint,2,5,1);
//pid

unsigned long time;


void setup() 
{  
	//gyro
	analogReference(DEFAULT);
	//pinMode(warnled,OUTPUT);
	//pinMode(goled,OUTPUT);
	Serial.begin(9600);
	Wire.begin(); i = 0; 
	//gyro

	

	//PID 
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
	//PID
   //datalogger
   pinMode(txPin, OUTPUT);
   delay(1000);
   	mySerial.begin(19200);     
   	delay(10);
   	mySerial.print("Control on(1)/off(0),time,Servo pos., analog gyro in, gyro pos. gyro rateofchange,temp ");
   	//delay(1000);
   	
   	
}

void loop()
{

	for(int other_timer=0; other_timer < 120; other_timer++)//2min gps/temp reading
	{   
		mySerial.print("0");//signify spin control not active
         mySerial.print(",");

		//insert print time function
             time=millis();
		mySerial.print(time, DEC);
			mySerial.print(",");
		mySerial.print("0");//break for servo pos.
		mySerial.print(",");
		   mySerial.print(analogRead(0), DEC); 



		Gyro();

		mySerial.print(",");
	    	mySerial.print(printposition, BIN);
            mySerial.print(",");
		    mySerial.print(printrateofchange,BIN);

		temprature();
	
		delay(1000);
	}


	for(int gyro_timer=0; gyro_timer < 300; gyro_timer++)//5min gyro contorl
	{  
		mySerial.print("1");//signify spin control active
		         mySerial.print(",");

		int loopnum=0;
		time=millis();
		mySerial.print(time, DEC );
		//insert print time function
	    Gyro();
		for(int pidloop=0; pidloop<10;pidloop++)
        { 
           if(pidloop==9)
            {
            loopnum = 1;
			}

			PIDCONTROL(loopnum);
			delay(100);
  
        }
        	mySerial.print(",");
	    	mySerial.print(printposition, BIN);
            mySerial.print(",");
		    mySerial.print(printrateofchange,BIN);
			mySerial.print(",");
			mySerial.println("0");
	}	
	

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
	//	Serial.print(val*0.0625);/* convert to Deg C(>0!)*/
	 mySerial.print(",");
		mySerial.println(val*0.0625, BIN);
                //delay(1000); 
	}

}




void Gyro()//Read Gyro data
{

  lasttook = timetook;
  int rate = analogRead(0);
  timetook = millis();
  
  /*  run 1000 samples of the gryo
  and find out how much jitter we are expecting
  throw up an error if its more than 5 */
  while(loops < 1000)
  {
    int sample = analogRead(sensor);
    if(sample > high)
    {
      high = sample;
    }
    else if(sample < low)
    {
      low = sample;
    }
    loops++;
    delay(1);  

  }
  
  if(loops==1000)
  {
    centerpoint = (high-low)+low;  
    variation = high-low; 
  }


  // simple statement to keep a little of the natural jitter out
  if(abs(centerpoint-rate) > variation)
  {
    //float rateofchange;
    change = centerpoint-rate;
    float GYRO_SENSITIVITY;
    /* qucik math */
    if(change >= 0)
      GYRO_SENSITIVITY = 0.6989;//0.69373; //CCW spin
    if(change < 0)
      GYRO_SENSITIVITY = 0.6757197; //0.67703; //CW spin
    
    float volts = float(change);//*0.001201172;
     float rateofchange = volts/GYRO_SENSITIVITY;    
    
    
    float time = (float(timetook)-float(lasttook))/1000.0;
    float degchange = rateofchange*time;
    printrateofchange = rateofchange;
   
   
    position = position+degchange;
    position = makecardinal(position);
    printposition = position;
  //  if(loopnum==1)
//

		// mySerial.print(",");
	//	mySerial.println(printrateofchange,BIN);
//	}
  }

   
	Serial.println(position);
	change=0;  
}
  //****************insert print function here 
  //print possition
  //print rateofchange
  
  
  
  
 // delay(10);

//simple function to make the poisition make sense
	float makecardinal(float position)
	{
		if(position > 360.00)
		{
			position = position-360.00;
		}
		else if(position < 0)
		{
			position = 360+position;
		}
		return position;	
	}




int PIDCONTROL(int loopnum)//PID control
{
	
	Input = analogRead(0);
	myPID.Compute();
	//analogWrite(3,Output);
	Serial.print(analogRead(0));
	Serial.print("  ");
    Serial.print(Output);
  
	 if(Input <= Setpoint - 2 || Input >= Setpoint + 2) //dead zone
 {
	 Serial.print("Dead zone");
	// angle = 60
 } 
 if(Input > Setpoint + 2) 
 {
	Serial.print("more then");
    angle = 60 - Output;
   
 }
 if(Input < Setpoint - 2)
 {
   Serial.print("less then");
    angle = Output + 60 ;

 }
    
    myservo1.write(angle);
	myservo2.write(angle);
	Serial.print("  ");
    Serial.print(angle);
     
 if(loopnum==1)
 {
   mySerial.print(",");
    mySerial.print(angle, BIN);
   mySerial.print(",");
    mySerial.print(analogRead(0), DEC);
    
 }
    //*****************************insert print angle
    
    
	//delay(10);
}
