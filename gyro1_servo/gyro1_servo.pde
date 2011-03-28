//#define GYRO_SENSITIVITY 0.00121;
//#define GYRO_SENSITIVITY 0.6938;

#include <Servo.h> 
 
Servo myservo; 
int loops=0; //configuration stage loops
int centerpoint = 0; // zero rotation value
int high = 0; // motionless calibration error
int low = 1023; // motionless calibration error

int warnled = 2; // warning led pin
int goled = 3; // systems green pin
int sensor = 0; // analog gyro port
int variation = 0; // expected gyro jitter +- value
int change = 0; // ATD conversion change
float position = 0.0; // current cardinal position
unsigned long timetook = 0; //sample time taken
unsigned long lasttook = 0; //last sample time

void setup(){
  analogReference(DEFAULT);
  pinMode(warnled,OUTPUT);
  pinMode(goled,OUTPUT);
  myservo.attach(10);
  Serial.begin(9600);
}

void loop(){
  lasttook = timetook;
  int rate = analogRead(0);
  timetook = millis();
  
  /* lets quickly run 1000 samples of the gryo
  and find out how much jitter we are expecting
  throw up an error if its more than 5 */
  while(loops < 1000){
    digitalWrite(warnled,HIGH);
    int sample = analogRead(sensor);
    
    if(sample > high){
      high = sample;
    }else if(sample < low){
      low = sample;
    }
    loops++;
    delay(1);  

  }
  
  
  if(loops==1000){
    digitalWrite(warnled,LOW);
    centerpoint = (high-low)+low;  
    variation = high-low;
    if(variation > 5){
      digitalWrite(warnled,HIGH);
    }else{
      digitalWrite(goled,HIGH);
    }
  }


  // simple statement to keep a little of the natural jitter out
  if(abs(centerpoint-rate) > variation){
   //  float rateofchange;
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
  
  delay(10);
}


//simple function to make the poisition make sense
float makecardinal(float position){
  if(position > 360.00){
      position = position-360.00;
    }else if(position < 0){
      position = 360+position;
    }
    return position;
}
 
