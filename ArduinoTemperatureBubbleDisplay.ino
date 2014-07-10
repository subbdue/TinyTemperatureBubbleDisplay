/*
 March 6, 2014
 Spark Fun Electronics
 Nathan Seidle
 Updates by Joel Bartlett
 
 This code is originally based Dean Reading's Library deanreading@hotmail.com
 http://arduino.cc/playground/Main/SevenSegmentLibrary
 He didn't have a license on it so I hope he doesn't mind me making it public domain: 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This sketch provides a simple counter example for the HP Bubble display from SparkFun.
 https://www.sparkfun.com/products/12710
 
 Pinout for HP Bubble Display:
 1:  Cathode 1
 2:  Anode E
 3:  Anode C
 4:  Cathode 3
 5:  Anode dp
 6:  Cathode 4
 7:  Anode G
 8:  Anode D
 9:  Anode F
 10: Cathode 2
 11: Anode B
 12: Anode A
 */

//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures


#include "SevSeg.h"

//Create an instance of the object.
SevSeg myDisplay;

//Create global variables
unsigned long timer;
int deciSecond = 0;
int temperature = 0;
int interval = 0;
char tempString[4];

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
 
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
                       
  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  
  //This pinout is for a bubble dispaly
  //Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 8; //Pin 1
  int digit2 = 5; //Pin 10
  int digit3 = 11; //Pin 4
  int digit4 = 13; //Pin 6
  
  //Declare what pins are connected to the segments (anodes)
  int segA = 7; //Pin 12
  int segB = 6; //Pin 11
  int segC = 10; //Pin 3
  int segD = 3; //Pin 8
  int segE = 9; //Pin 2
  int segF = 4; //Pin 9
  int segG = 2; //Pin 7
  int segDP= 12; //Pin 5
   
  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  timer = millis();
}
 
void loop()                     // run over and over again
{
   // At 500 display Centigrade, at 1000 
   // .. display Faranheit, clear interval
   // .. cnt at 1000
   if(interval == 500)
   {
     temperature = getTemp(0);
     sprintf(tempString, "%1s%3d", "C", temperature);
     interval++;
   }
   else if(interval == 1000)
   {
     temperature = getTemp(1);
     sprintf(tempString, "%1s%3d", "F", temperature);
     interval = 0;
   }
   else
   {
     interval++;
   }
   
   // Display
   myDisplay.DisplayString(tempString, 0); 
   delay(5); //waiting a second
}

int getTemp(boolean tempType)
{
   //getting the voltage reading from the temperature sensor
   int reading = analogRead(sensorPin);  
   
   // converting that reading to voltage, for 3.3v arduino use 3.3
   float voltage = reading * 5.0;
   voltage /= 1024.0; 
   
   // print out the voltage
   Serial.print(voltage); Serial.println(" volts");
   
   // now print out the temperature
   // converting from 10 mv per degree wit 500 mV offset
   // to degrees ((voltage - 500mV) times 100)
   float temperatureC = (voltage - 0.5) * 100 ;
                                             
   Serial.print(temperatureC); Serial.println(" degrees C");
   
   // now convert to Fahrenheit
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
   Serial.print(temperatureF); Serial.println(" degrees F");
   if(tempType == 0)
   {
      return int(temperatureC);  
   }
   else
   {
     return int(temperatureF);  
   } 
}
