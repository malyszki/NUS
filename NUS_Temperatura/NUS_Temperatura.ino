#include <LiquidCrystal.h>
#define TEMP 13
#define FAN 44
#define HEATER0 10
#define HEAD 

#define A -6.458565579810880e-07
#define B 0.001142619771655
#define C -0.758914926930814
#define D 2.765483204012756e+02

int temp;
double real_temp;
//int i = 0;
double u = 0;
double integrate = 0;
double e = 0;

LiquidCrystal lcd (16,17, 23, 25, 27, 29);
void setup(){
  
  Serial.begin(9600);
  lcd.begin(20,4);

  pinMode(HEATER0,OUTPUT);
  digitalWrite(HEATER0,LOW);

  pinMode(FAN,OUTPUT);
  digitalWrite(FAN,HIGH);
}

void loop()
{
  temp = analogRead (TEMP);
  real_temp = A * (double)pow(temp, 3) + B * (double) pow (temp, 2) + C * (double) temp + D;

  e = 200 - real_temp;
  integrate = integrate + e;
  u = 255/180*e + integrate/70;
  
  if (real_temp < 200)
  {
    analogWrite(HEATER0, u > 255.0 ? 255 : floor(u));    
  }
  else
  {
    digitalWrite(HEATER0, LOW);   
  }
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(7,0);
  lcd.print(real_temp);
  lcd.setCursor(0,1);
  lcd.print("Blad:");
  lcd.setCursor(7,1);
  lcd.print(e);
  lcd.setCursor(0,2);
  lcd.print("Calka:");
  lcd.setCursor(7,2);
  lcd.print(integrate);
  lcd.setCursor(0,3);
  lcd.print("Ster:");
  lcd.setCursor(7,3);
  lcd.print(u);
  delay(10);
}

