#include "definitions.h"

///////////
void writeAZ0()
{                      // write a '0' bit toneburst
 for (int i=1; i<=22; i++){         // 1 ms of 22 kHz (22 cycles)
  digitalWrite(datapinAZ,HIGH);
  delayMicroseconds(16);
  digitalWrite(datapinAZ,LOW);
  delayMicroseconds(17);
 }
delayMicroseconds(500);             // 0.5 ms of silence
}
///////////
void writeAZ1()
{                      // write a '1' bit toneburst
 for (int i=1; i<=11; i++)
 {         // 0.5 ms of 22 kHz (11 cycles)
  digitalWrite(datapinAZ,HIGH);
  delayMicroseconds(16);
  digitalWrite(datapinAZ,LOW);
  delayMicroseconds(17);
 }
delayMicroseconds(1000);            // 1 ms of silence
}
//////////
// write the parity of a byte (as a toneburst)
// note parity_even_bit(B1101101) returns 1 (odd parity)
//      parity_even_bit(B1101001) returns 0 (even parity)
void write_parity_AZ(byte x)
{
 if (parity_even_bit(x)) writeAZ0(); else writeAZ1();
}

//////////////////
// write out a byte (as a toneburst)
// high bit first (ie as if reading from the left)
void write_byte_AZ(byte x)
{
  for (int j=7; j>=0; j--){
  if (x & (1<<j)) writeAZ1(); else writeAZ0();
   }
}
////////////////
// write out a byte with parity attached (as a toneburst)
void write_byte_with_parity_AZ(byte x)
{
  write_byte_AZ(x);
  write_parity_AZ(x);
}
//////////////////////////////////////////////////////////////
// goto position angle a in degrees, south = 0.
// (a must be in the range +/- 75 degrees)
void goto_angle(float a)
{
/*
Note the diseqc "goto x.x" command is not well documented.  
The decription in http://www.eutelsat.com/satellites/4_5_5.html 
is for 'terrestrial positioners' rather than satellite positioners
Which drives use the terrestial command set and which use the satellite
set is not clear, but various web resources, including 
http://www.techwatch.co.uk/forums/14112-raw-diseqc-1-2-commands-a-rough-guide.html
indicate the following is right for the satellite set:
*/
 float fa16;
 byte n1,n2,n3,n4,d1,d2;
 int a16;
// get the angle in range +/- 75 degrees.  Sit at these limits and switch
// over at ~ midnight unless otherwise instructed.

if (a <-75.0) {a=-75;}
if (a > 75.0) {a= 75;}

// set the sign nibble in n1 to E (east) or D (west). 
if (a<0) { n1=0xE0;} else {n1=0xD0;}
// shift everything up so the fraction (1/16) nibble is in the 
//integer, and round to the nearest integer:
a16 =  (int) (16.0*abs(a)+0.5); 
// n2 is the top nibble of the three-nibble number a16:
n2 = (a16 & 0xF00)>>8;
// the second data byte is the bottom two nibbles:
d2 = a16 & 0xFF;
//the first data byte is
d1 = n1 | n2;
// send the command to the positioner
  noInterrupts();
  write_byte_with_parity_AZ(0xE0);
  write_byte_with_parity_AZ(0x31);
  write_byte_with_parity_AZ(0x6E);
  write_byte_with_parity_AZ(d1);
  write_byte_with_parity_AZ(d2);
  interrupts();
}
//////////////////////////////////////////////////////////////////
