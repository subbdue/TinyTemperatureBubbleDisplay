/*
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
int sensorPin = A0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures

// #########################################
// Shrinking down SevSeg.h for Attiny
// #########################################
#include "SevSeg.h"
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#define FRAMEPERIOD 4000
//Create an instance of the object.
SevSeg myDisplay;

//Create global variables
//unsigned long timer;
int deciSecond = 0;
int temperature = 0;
int interval = 0;
char tempString[4];

/**
 * Attiny84 pin definitions
 * http://iamsuhasm.wordpress.com/tutsproj/avr-gcc-tutorial/
 * Use DDRx (DDRB, DDRD, etc) to set Attiny IOs as input or output
 * Use PORTx (PORTB, PORTD, etc) to drive the IO
 */
//#define sensorPin A0
//#define digit1 PA6
//#define digit2 100
//#define digit3 PB1
//#define digit4 PB0
//
//#define segA PA1
//#define segB PA2
//#define segC PB2
//#define segD PA4
//#define segE PA7
//#define segF PA3
//#define segG PA5
//#define segDP 100

void setup()
{
  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  
  //This pinout is for a bubble dispaly
  //Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 6; //Pin 1
  int digit2 = 101; //Pin 10
  int digit3 = 9; //Pin 4
  int digit4 = 10; //Pin 6
   
  //Declare what pins are connected to the segments (anodes)
  int segA = 1; //Pin 12
  int segB = 2; //Pin 11
  int segC = 8; //Pin 3
  int segD = 4; //Pin 8
  int segE = 7; //Pin 2
  int segF = 3; //Pin 9
  int segG = 5;//5; //Pin 7
  int segDP= 100; //Pin 5
   
  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?
  
  //DDR - DataDirectionRegister
  DDRA |= (1<<DDA1) | (1<<DDA2) | (1<<DDA3) | (1<<DDA4) |
          (1<<DDA5) | (1<<DDA6) | (1<<DDA7);
  DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  //timer = millis();
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
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)

 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 if(tempType == 0)
 {
    return int(temperatureC);  
 }
 else
 {
   return int(temperatureF); 
 } 
}
