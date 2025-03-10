#include <ADF4156_250.h>
#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);   // UNO

void setup() 
{ 
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("AD4156");
  ADF4156.begin();
}

unsigned long t1,t2,f; 
unsigned int m;   

void loop() 
{
  delay(100);
  t1=millis();
  lcd.clear();
  f=270000000;
  ADF4156.imposta_f(f);
  lcd.print("f="+String(f)); 
 
 /*
  for (f=27000000; f<320000000; f=f+10000)
    {
    //f=84500000;  // 10MHz in ricezione al front-end
    ADF4156.imposta_f(f);
    lcd.print("f="+String(f));
    }
 */  
  //
   //
  /*
  ADF4156.scansione(54500000,84500000,100); // da 45 a 75MHz con IF=45.000MHz
  t2=millis();
  lcd.print(String(t2-t1));
  */
  
  while(true){};
}


