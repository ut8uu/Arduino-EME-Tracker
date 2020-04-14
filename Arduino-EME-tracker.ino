// e_callisto_drive uses an arduino to control a DiSEqC 1.2 satellite motor to
// an antenna can track the Sun in right ascension.
// The arduino calculates the position of the Sun and sends tone controls to 
// the drive via pin 8.  Extrernal electronics are required to inject the tone 
// signals onto the drive power lines.
//
// Single precision limits the angular precision. A comparison with the JPL Horizons
// system show discrete errors of up to 1 minute in hour angle without the lookup tables.
// With the tables the errors are less (not checcked how much less!)
//
// 22 kHz signal should be set to 650 mV p-p
// see http://www.eutelsat.com/satellites/pdf/Diseqc/Reference%20docs/bus_spec.pdf
// and http://www.eutelsat.com/satellites/pdf/Diseqc/associated%20docs/position_app_note_v1.pdf
// but note these are incomplete.
//
// Set the time at the serial console (9600 baud) with the format "yyyy mm dd hh mm ss".
//
// Graham Woan 13/10/2012
// still to do:
// display sidereal time
#include <Time.h>
#include <math.h>
#include  <util/parity.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#include "definitions.h"
#include "functions.h"
#include "rw.h"


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

SoftwareSerial mySerial(6, 4); // from the GPS module, green to 6, white to 4

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

float longitude = -4.30722; // default telescope longitude, degrees, E positive
float latitude = 55.90222; // default telescope latitude, degrees north
float alat;
int sw5v = 3; // power pin for the switch
int sw = 2; // switch pin
int switchstate = 0;  // variable for reading the switch status

float oldAZ = 0.1;
float currentAZ = 0.0;
float oldEL = 0.1;
float currentEL = 0.0;

int button = BTN_NONE;

//////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(datapinAZ,OUTPUT); // tone output
  pinMode(datapinEL,OUTPUT); // tone output
  pinMode(sw5v, OUTPUT); // power pin for switch
  digitalWrite(sw5v, HIGH); 
  pinMode(sw, INPUT); //pin for switch

  lcd.begin (16,2);
  
  lcd.clear();
  lcd.home ();                   // go home
  lcd.setCursor (0,0);
  lcd.print("   EME DiSEqC   "); 
  lcd.setCursor (0,1);
  lcd.print("  tracker v1.0  "); 
  
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(9600);

  delay(1000);
}
/////////////////////////////////////////////////////////////////////////////


uint32_t timer = millis();
/////////////////////////////////////////////////////////////////////////////
float reduce(float x) 
{ //reduce and angle to the range (0,360)
 while (x>360.0) {
   x = x-360.0;
 }
 while (x<0.0) {
   x = x+360.0;
 }
 return(x);
}
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
// main loop ////////////////////////////////////////////////////
//
void loop() 
{
   delay(100);

   char ln[16];
   lcd.home();

   button = CheckForButton();

   switch (button)
   {
    case BTN_UP:
      currentEL = Increment(currentEL, 0.1);
      break;
    case BTN_DOWN:
      currentEL = Decrement(currentEL, 0.1);
      break;
    case BTN_LEFT:
      currentAZ = Decrement(currentAZ, 0.1);
      break;
    case BTN_RIGHT:
      currentAZ = Increment(currentAZ, 0.1);
      break;
    default:
      break;
   }

   lcd.clear();
   lcd.home ();                   // go home

   PrintAZ(lcd, currentAZ);
   PrintEL(lcd, currentEL);

   return;
    
   if (oldAZ!=currentAZ) 
   {
     lcd.setCursor (0,0);
     char chOld[5];
     char chCurrent[5];
     dtostrf(oldAZ*15.0, 6, 1, chOld);
     dtostrf(currentAZ*15.0, 6, 1, chCurrent);
     lcd.print("AZ ");
     lcd.print(chOld); 
     lcd.print(" > ");
     //lcd.print((char)223);
     lcd.print(chCurrent);
     // send command to the diseqc motor, converting hours to degrees
     goto_angle(currentAZ*15.0);
     if (oldAZ<500.0) 
     {
      delay(8000.0*abs(currentAZ-oldAZ));
     } 
     else 
     {
        delay(8000.0*abs(currentAZ));
     }
     oldAZ = currentAZ;
   }
    
   if (oldEL!=currentEL) 
     {
     lcd.setCursor (0,1);
     char chhh[5];
     dtostrf(currentAZ*15.0, 6, 1, chhh);
     lcd.print("drive to EL");
     lcd.print(chhh); 
     lcd.print((char)223);
     // send command to the diseqc motor, converting hours to degrees
     goto_angle(currentAZ*15.0);
     if (oldAZ<500.0) {delay(8000.0*abs(currentAZ-oldEL));} else {delay(8000.0*abs(currentEL));}
     oldEL = currentAZ;
     }

}
