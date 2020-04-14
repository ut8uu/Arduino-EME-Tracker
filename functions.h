
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

/////////////////////////////////////////////////////////////////////////////

void ClearLine(LiquidCrystal lcd, int row)
{
  lcd.setCursor(0, row);
  lcd.print("                ");
}

/////////////////////////////////////////////////////////////////////////////

void PrintFloatCont(LiquidCrystal lcd, float f)
{
     char s[5];
     dtostrf(f, 2+log(floor(f)), 1, s);
     lcd.print(s);
}

void PrintAZ(LiquidCrystal lcd, float az)
{
     ClearLine(lcd, 0);
     lcd.setCursor (0,0);
     lcd.print("AZ ");
     PrintFloatCont(lcd, az);
}

void PrintAZmove(LiquidCrystal lcd, float old_az, float az)
{
     ClearLine(lcd, 0);
     lcd.setCursor (0,0);
     lcd.print("AZ ");
     PrintFloatCont(lcd, old_az);
     lcd.print(" > ");
     PrintFloatCont(lcd, az);
}

void PrintEL(LiquidCrystal lcd, float el)
{
     ClearLine(lcd, 1);
     lcd.setCursor (0,1);
     lcd.print("EL ");
     PrintFloatCont(lcd, el);
}

void PrintELmove(LiquidCrystal lcd, float old_el, float el)
{
     ClearLine(lcd, 1);
     lcd.setCursor (0,1);
     lcd.print("EL ");
     PrintFloatCont(lcd, old_el);
     lcd.print(" > ");
     PrintFloatCont(lcd, el);
}
