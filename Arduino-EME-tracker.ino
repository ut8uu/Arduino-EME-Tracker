#include <Time.h>
#include <math.h>
#include  <util/parity.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#include "definitions.h"
#include "functions.h"
#include "rw.h"


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

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

  delay(2000);
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

   bool need_pause = false;

   if (oldAZ!=currentAZ) 
   {
     PrintAZmove(lcd, oldAZ, currentAZ);
     // send command to the diseqc motor, converting hours to degrees
     goto_angle(currentAZ);
     oldAZ = currentAZ;
     need_pause = true;
   }
    
   if (oldEL!=currentEL) 
     {
     PrintELmove(lcd, oldEL, currentEL);
     // send command to the diseqc motor, converting hours to degrees
     goto_angle(currentEL);
     oldEL = currentEL;
     need_pause = true;
     }

   if (need_pause)
   {
   delay(250);
   lcd.clear();
   lcd.home ();                   // go home
   PrintAZ(lcd, currentAZ);
   PrintEL(lcd, currentEL);
   }


}
