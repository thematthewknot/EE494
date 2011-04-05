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
*/

#include <NewSoftSerial.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "Wire.h"
#include <ctype.h>
#include <Servo.h>
#include <PID_Beta6.h>


void Temprature();
void GPS();
void Gyro();
void PID();


//datalogger
NewSoftSerial mySerial(rxPin, txPin);
#define rxPin 4 
#define txPin 6 
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
//gyro


//gps
// int ledPin = 13;                  // LED test pin
 int GrxPin = 0;                    // RX PIN 
 int GtxPin = 1;                    // TX TX
 int byteGPS=-1;
 char linea[300] = "";
 char comandoGPR[7] = "$GPRMC";
 int cont=0;
 int bien=0;
 int conta=0;
 int indices[13];
//gps

//pid

double Setpoint, Input, Output,angle;
Servo myservo1, myservo2; 
PID myPID(&Input, &Output, &Setpoint,2,5,1);
//pid

void setup() 
{  
	//gyro
	analogReference(DEFAULT);
	pinMode(warnled,OUTPUT);
	pinMode(goled,OUTPUT);
	Serial.begin(9600);
	Wire.begin(); i = 0; 
	//gyro
	
	//gps
	//pinMode(ledPin, OUTPUT);       // Initialize LED pin
	pinMode(GrxPin, INPUT);
	pinMode(GtxPin, OUTPUT);
	Serial.begin(4800);
	for (int i=0;i<300;i++)
	{       // Initialize a buffer for received data
		linea[i]=' ';
	}
	//gps
	
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
   
   
}

void loop()
{
	
	for(int gyro_timer=0; gyro_timer < 300; gyro_timer++)//5min gyro contorl
	{
		
		//insert print time function
		Gyro();
		PID();
		delay(1000);
	}
	
	for(int other_timer=0; other_timer < 120; other_timer++)//2min gps/temp reading
	{
		//insert print time function
		Gyro();
		temprature();
		GPS();
		delay(1000);
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
		Serial.print(val*0.0625);/* convert to Deg C(>0!)*/
		//delay(1000); 
	}
}


void GPS()//Read GPS data.
{

   //digitalWrite(ledPin, HIGH);
   byteGPS=Serial.read();         // Read a byte of the serial port
   if (byteGPS == -1) {           // See if the port is empty yet
     delay(100); 
   }
   else 
   {
     linea[conta]=byteGPS;        // If there is serial port data, it is put in the buffer
     conta++;                      
     Serial.print(byteGPS, BYTE); 
     if (byteGPS==13)            // If the received byte is = to 13, end of transmission
     {
		//digitalWrite(ledPin, LOW); 
		cont=0;
		bien=0;
		for (int i=1;i<7;i++)     // Verifies if the received command starts with $GPR
        { 
			if (linea[i]==comandoGPR[i-1])
			{
           bien++;
			}
		}
       
		if(bien==6) // If yes, continue and process the data
		{              
         for (int i=0;i<300;i++)
         {
           if (linea[i]==',') // check for the position of the  "," separator
           {   
             indices[cont]=i;
             cont++;
           }
           if (linea[i]=='*')    // ... and the "*"
           {  
             indices[12]=i;
             cont++;
           }
         }
         
         Serial.println("");      // ... and write to the serial port
         Serial.println("");
         Serial.println("---------------");
         for (int i=0;i<12;i++){
           switch(i){
             case 0 :Serial.print("Time in UTC (HhMmSs): ");break;
             case 1 :Serial.print("Status (A=OK,V=KO): ");break;
             case 2 :Serial.print("Latitude: ");break;
             case 3 :Serial.print("Direction (N/S): ");break;
             case 4 :Serial.print("Longitude: ");break;
             case 5 :Serial.print("Direction (E/W): ");break;
             case 6 :Serial.print("Velocity in knots: ");break;
             case 7 :Serial.print("Heading in degrees: ");break;
             case 8 :Serial.print("Date UTC (DdMmAa): ");break;
             case 9 :Serial.print("Magnetic degrees: ");break;
             case 10 :Serial.print("(E/W): ");break;
             case 11 :Serial.print("Mode: ");break;
             case 12 :Serial.print("Checksum: ");break;
           }
           for (int j=indices[i];j<(indices[i+1]-1);j++)
           {
             Serial.print(linea[j+1]); 
           }
           Serial.println("");
         }
         Serial.println("---------------");
       }
       conta=0;                    // Reset the buffer
       for (int i=0;i<300;i++)
       {    
         linea[i]=' ';             
       }                 
     }
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
    
    position = position+degchange;
    position = makecardinal(position);
  }
  
  Serial.println(position);
  change=0;  
  
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
}
	

	
void PID()//PID control
{
	Input = analogRead(0);
	myPID.Compute();
	//analogWrite(3,Output);
	Serial.print(analogRead(0));
	Serial.print("  ");
    Serial.print(Output);
  
	if(Input>=Setpoint)
	{
		Serial.print("more then");
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
    
    //*****************************insert print angle
    
    
	//delay(10);
}
	 
