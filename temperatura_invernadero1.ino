#include <Ds1302.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"



int s1 = 0;
int s2 = 0;
int s3 = 0;
int s1lcd = 0;
int s2lcd = 0;
int s3lcd = 0;
int wantedtemp = 20;
int openedWindow = 0;


Servo servo;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
//Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);
Ds1302 rtc(5, 7, 6);
DHT dht0(A0, DHT11);
DHT dht1(A1, DHT11);
DHT dht2(A2, DHT11);

void setup()				
{
  rtc.init();
  lcd.init();
  lcd.backlight();
  Wire.begin();
  dht0.begin();
  dht1.begin();
  dht2.begin();
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
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
  s1 = dht0.readTemperature();
  s2 = dht1.readTemperature();
  s3 = dht2.readTemperature();
  
  
  if (s2 != wantedtemp) {
    
    if (s2 < wantedtemp) { //look for hottest
      
      if (s1 > s3) {
        
        if (s1 > s2) {
        	//exec s1
        	digitalWrite(13,HIGH);//HIGH= 5v
        }
        else {
        	digitalWrite(13, LOW);
        }
      }
      if (s3 > s1) {
        if (s3 > s2) {
        	//exec s1
        	digitalWrite(12,HIGH);//HIGH= 5v
        }
        else {
        	digitalWrite(12, LOW);
        }
      	}
      
    }
    
    if (s2 > wantedtemp) { //look for coldest
      if (s3 < s1) {
        if (s3 < s2){
          digitalWrite(12,HIGH);//HIGH= 5v
        }
        else {
        digitalWrite(12, LOW);
        }
      }
      if (s1 < s3) {
        if (s1 < s2){
          digitalWrite(13,HIGH);//HIGH= 5v
        }
        else {
          digitalWrite(13, LOW);
        }
      }
    } 
  }
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  
  lcd.setCursor(1,0);
  String s2c = "I:" + String(s2) + String((char)223) + "C ";
  lcd.print(s2c);
  delay(50);
  
  lcd.setCursor(9,0); 
  String s3c = "O:" + String(s3) + String((char)223) + "C ";
  lcd.print(s3c);
  delay(50);
  
  lcd.setCursor(1,1);
  String s1c = "H:" + String(s1) + String((char)223) + "C ";
  lcd.print(s1c);
  delay(50);
  
  lcd.setCursor(9,1);
  String wantedlcd = "D:" + String(wantedtemp) + String((char)223) + "C ";
  lcd.print(wantedlcd);
  delay(50);

  if (digitalRead(1) == HIGH) {
    wantedtemp = wantedtemp + 1;
    Serial.print("+");
  }else {
    Serial.print("=");
  }
  if (digitalRead(0) == HIGH) {
    wantedtemp = wantedtemp - 1;
    Serial.print("-");
  }else {
    Serial.print(":");
  }
  delay(200);

  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  if (s2 > 20) {
    if (openedWindow == 0) {
      servo.write(90);
      openedWindow = 1;
      }
    }
  if (s2 < 15) {
    if (openedWindow == 1) {
      servo.write(0);
      openedWindow = 0;
    }
  }
   
  if (now.hour < 17 && now.hour < 19) {
    if (openedWindow == 0) {
      servo.write(90);
      openedWindow = 1;
    }
  }
  else {
    if (openedWindow == 1) {
      servo.write(0);
      openedWindow = 0;
    }
  }
}
  
