//digitalWrite() can be used for relays as well so if you wan to use a 12v fan instead of a led you can use it no problem
//git repo; https://github.com/loloide/Arduino-Greenhouse
//you need the following libraries: Ds1302(clock), Servo(servos), Wire, LiquidCrystal_I2C(display (4 pins)), DHT(temperature and humidity sensors)

#include <Ds1302.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"


int s1 = 0;
int s2 = 0;
int s3 = 0;
int wantedtemp = 20;
int openedWindow = 0;

Servo servo;

//display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

//Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);
//remember to set the hour the first time you run the arduino and then delete or comment the command later
Ds1302 rtc(5, 7, 6);

//DHT dht(pin, model);
DHT dht0(A0, DHT11);
DHT dht1(A1, DHT11);
DHT dht2(A2, DHT11);

void setup()        
{
  //start the different modules
  rtc.init();
  lcd.init();
  lcd.backlight();
  Wire.begin();
  dht0.begin();
  dht1.begin();
  dht2.begin();
  Serial.begin(9600);
  
  //we dont need to declare the inputs because we already did in the "DHT dht();" functions
  
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  
  servo.attach(9);

  
}

void loop()
{
  //reads the temperature from the sensors
  s1 = dht0.readTemperature();
  s2 = dht1.readTemperature();
  s3 = dht2.readTemperature();
  
  
  if (s2 != wantedtemp) {
    
    if (s2 < wantedtemp) { //look for hottest
      
      if (s1 > s3) {
        //exec s1
        digitalWrite(13,HIGH);//HIGH= 5v
      }
      else {
        digitalWrite(13,LOW);
      }
      
      
      if (s3 > s1) {
        //exec s3
        digitalWrite(12,HIGH);//HIGH= 5v
      }
      else {
        digitalWrite(12,LOW);
      }
    }
    
    if (s2 > wantedtemp) { //look for coldest

      if (s1 < s3) {
        //exec s1
        digitalWrite(13,HIGH);//HIGH= 5v
      }
      else {
        digitalWrite(13,LOW);
      }
      
      if (s3 < s1) {        
        //exec s3
        digitalWrite(12,HIGH);//HIGH= 5v
      }
      else {
        digitalWrite(12,LOW);
      }
      
    } 
  }
  
  //print in the display
  //|s2 wantedtemp|
  //|s1         s3|
  lcd.setCursor(1,0);
  String s2c = "I:" + String(s2) + String((char)223) + "C ";
  lcd.print(s2c);
  delay(50);

  lcd.setCursor(9,0);
  String wantedlcd = "D:" + String(wantedtemp) + String((char)223) + "C ";
  lcd.print(wantedlcd);
  delay(50);
  
  lcd.setCursor(1,1);
  String s1c = "H:" + String(s1) + String((char)223) + "C ";
  lcd.print(s1c);
  delay(50);
  
  lcd.setCursor(9,1); 
  String s3c = "O:" + String(s3) + String((char)223) + "C ";
  lcd.print(s3c);
  delay(50);

  lcd.setCursor(0,0);
  lcd.print(openedWindow);


  //change wantedtemp with the buttons
  if (digitalRead(1) == HIGH) {
    wantedtemp = wantedtemp + 1;
  }
  if (digitalRead(0) == HIGH) {
    wantedtemp = wantedtemp - 1;
  }
  delay(200);

  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  //move window with the clock
  if (now.hour > 16 && now.hour < 18) {
      servo.write(0);//open
      openedWindow = 1;
  }
  else {
    //move window with the temperature
    if (s2 > 20) {
      servo.write(0);//open
      openedWindow = 1;
    }
    else if (s2 < 15) {
      servo.write(180);//close
      openedWindow = 0;
    }  
    else {
      servo.write(180);//close
      openedWindow = 0;
    }
  }
  Serial.print(now.hour);
  Serial.print(" ");
  Serial.print(now.minute);
  Serial.print("  ");
}