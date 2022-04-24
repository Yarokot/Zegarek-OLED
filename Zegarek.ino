#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <DS3231.h>

DS3231 clock;
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
RTClib myRTC;
//RTCDateTime dt;

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
OneWire oneWire(A2); //Podłączenie do A5
DallasTemperature sensors(&oneWire); 

int var, u, t2, x, y, q, q2, y2, cleard, rr, h, m, d, mi,r,t;

unsigned long aCzas = 0;
unsigned long zCzas = 0;
unsigned long rCzas = 0;

unsigned long aCzas2 = 0;
unsigned long zCzas2 = 0;
unsigned long rCzas2 = 0;

void setup()   {  
      pinMode(7, INPUT_PULLUP);   //przelaczanie+
      pinMode(6, INPUT_PULLUP);   //przelaczanie-
      pinMode(8, INPUT_PULLUP);
      pinMode(9, OUTPUT);
      
var=1; 
h=0;
m=0;
d=0;
mi=0;
r=0;
rr=0;
x=0;
y=0;
q=0;
q2=0;
y2=0;
cleard=1;
t=1;

  Serial.begin(9600);
  sensors.begin();
  //clock.begin();
  //clock.setDateTime(__DATE__, __TIME__);
  Wire.begin();  
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)  
  display.setRotation(2);
  display.clearDisplay();
  display.setTextColor(WHITE); 
}
void data()
{
  cleard=1;
  display.clearDisplay();  
  display.setTextSize(4);
  display.setCursor(6,4);
  if (h<10)
  {
    display.print("0");
  }
  display.print(h);
  display.print(":");
  if (m<10)
  {
    display.print("0");
  }
  display.print(m);  
  display.setTextSize(2);
  display.setCursor(6,46);
  if (d<10)
  {
    display.print("0");
  }
  display.print(d);
  //display.setCursor(28,44);
  display.print("."); 
  //display.setCursor(40,44);
  if (mi<10)
  {
    display.print("0");
  }
  display.print(mi);
  //display.setCursor(64,44);
  display.print(".");
  //display.setCursor(76,44);
  display.print(r);
  display.display(); 
}

void temp()
{
  display.clearDisplay();
  display.setTextSize(1);
  //display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(4,4);
  display.print("IN:");
  display.setCursor(53,4);
  //int t=1;
  
  aCzas2 = millis();
  rCzas2 = aCzas2 - zCzas2;
  if (rCzas2 >= 10000UL)
  {
    var=1;
    zCzas2 = aCzas2;
  }
  
  //t=clock.getTemperature();
  t=t-2;
  aCzas = millis();
  rCzas = aCzas - zCzas;
  if (rCzas >= 2000UL)
  {
    sensors.requestTemperatures(); 
    t2=sensors.getTempCByIndex(0);
    zCzas = aCzas;
  }
  display.print(t);  
  //display.setCursor(95,4);
  display.setTextSize(1);
  display.print("o");
  display.setTextSize(3);
  display.println("C");  
  display.setTextSize(3);
  display.setCursor(4,36);
  display.print("OT:");
  display.setCursor(53,36);
  display.print(t2);
  //display.setCursor(95,42);
  display.setTextSize(1);
  display.print("o");
  display.setTextSize(3);
  display.println("C"); 
  display.display();
  //delay(2000);
}

void graph()
{
  if(cleard==1)
  {
    display.clearDisplay();
    cleard=0;
  }
  
  x=analogRead(A0);
  y = map(x, 0, 240, 61, 3); 
  //display.clearDisplay();  
  display.setTextSize(1);
  display.drawPixel(q, y, WHITE);
  display.drawLine(q2, y2, q, y, WHITE);
  //display.setCursor(z,y);
  //display.println(".");
  display.setCursor(50,10);
  //display.println(y);
  display.display();
  delay(50);
  q2=q;
  y2=y;
  q++;
  if(q==127)
  {
    q=0;   
    q2=0;  
    display.clearDisplay();
  }  
}

void loop() {
  //dt = clock.getDateTime();
  DateTime now = myRTC.now();
  h=now.hour(), DEC;
  m=now.minute(), DEC;
  d=now.day(), DEC;
  mi=now.month(), DEC;
  r=now.year(), DEC;
  t=clock.getTemperature();
  //rr=clock.dateFormat("s", dt);
  //if (rr == 30 or rr == 20 or rr == 10 or rr == 40 or rr == 50 or rr == 60 or rr == 0) display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  if (digitalRead(8) == LOW) 
  {
    digitalWrite(9, HIGH);
  }
  else
  {
    digitalWrite(9, LOW);
  }
  if(digitalRead(7)==LOW)
  {
    delay(20);
    if(digitalRead(7)==LOW and var<=2)
    {
      //if (var==3) var=1;
      if (var < 3) var++;    
      while (digitalRead(7)==LOW);    
    }
  }
  if(digitalRead(6)==LOW)
  {
    delay(20);
    if(digitalRead(6)==LOW and var>=2)
    {
      //if (var==1) var=3;
      if (var > 1) var--;    
      while (digitalRead(7)==LOW);    
    }
  }
  if(digitalRead(7)==HIGH)
  {   
    switch (var)
    {
      case 1:
      {
        data();
        break;
      }
      case 2:
      {
        temp();
        break;
      }
      case 3:
      {
        graph();
        break;
      }
    }
  }
}

  
