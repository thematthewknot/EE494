int x;  // X in-plane sensing

void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
}

void loop() 
{
  x = analogRead(0);       // read analog input pin 0
  
  Serial.print("rotational rates are x: ");
  Serial.print(x);    // print the rotational rate in the X axis
  Serial.print("\n");       // prints a space between the numbers
  
  delay(100);              // wait 100ms for next reading
}
