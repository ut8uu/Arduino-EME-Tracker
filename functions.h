
int CheckForButton()
{
  int keyAnalog =  analogRead(A0);
  if (keyAnalog < 100) 
  {
    return BTN_RIGHT;
  } 
  else if (keyAnalog < 200) 
  {
    return BTN_UP;
  } else if (keyAnalog < 400) 
  {
    return BTN_DOWN;
  } 
  else if (keyAnalog < 600) 
  {
    return BTN_LEFT;
  } 
  else if (keyAnalog < 800) 
  {
    return BTN_SELECT;
  } 

  return BTN_NONE;
}

float Increment(float value, float delta)
{
  return value + delta;
}

float Decrement(float value, float delta)
{
  return value - delta;
}

//////////////////////////////////////////////

float IncrementAZSmall(float az)
{
  return Increment(az, 0.1);
}

float DecrementAZSmall(float az)
{
  return Decrement(az, 0.1);
}

float IncrementELSmall(float el)
{
  return Increment(el, 0.1);
}

float DecrementELSmall(float el)
{
  return Decrement(el, 0.1);
}

void PrintAZ(LiquidCrystal lcd, float az)
{
     lcd.setCursor (0,0);
     char chCurrent[5];
     dtostrf(az, 6, 1, chCurrent);
     lcd.print("AZ ");
     lcd.print(chCurrent);
}

void PrintEL(LiquidCrystal lcd, float el)
{
     lcd.setCursor (0,1);
     char chCurrent[5];
     dtostrf(el, 6, 1, chCurrent);
     lcd.print("EL ");
     lcd.print(chCurrent);
}
