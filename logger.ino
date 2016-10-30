// sensor logeer
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN A1 
#define DHTTYPE DHT22
#define rain A0

File myFile;
unsigned long interval = 900000;
float t, h, rtc;
int r = 0;
long prev_min = 0;
String save_time, save_date, result, rain_res, t_file = "temp.txt", r_file = "rain.txt";

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  setSyncProvider(RTC.get);

  lcd.begin(16, 2);
  lcd.setCursor(0,0);

  if (!SD.begin(8)) {
    lcd.print("SD failed!");
    return;
  } else {
    lcd.print("SD present.");
  }
  //f_read(t_file);
}

void loop() {
  delay(500);
  t = dht.readTemperature();
  h = dht.readHumidity();
  r = analogRead(rain);
  int mr = map(r, 0, 1023, 100, 0);
  rtc = RTC.temperature() / 4.;
  save_time = String(hour())+digits(minute())+digits(second());
  save_date = String(day())+'-'+monthShortStr(month())+'-'+String(year());
  result = save_time +" "+ save_date +","+ String(t)+","+ String(h)+","+ String(rtc);
  rain_res  = save_time +" "+ save_date +","+ String(mr);

  if (minute() != prev_min) {
    prev_min = minute();
    f_write(t_file, result);
    f_write(r_file, rain_res);
  }
  lcd.setCursor(0,0);
  lcd.print(save_time);
  lcd.print(" ");
  lcd.print(t);
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print(mr);
  lcd.print("% ");
  lcd.print(r);
  lcd.print(" ");
  lcd.print(rtc);
  lcd.print("     ");
}
String digits(int d){
  String s = ":";
  if(d < 10)
    s += '0' + String(d);
  else s += String(d);
  return s;
}
void f_write(String file, String data) {
  myFile = SD.open(file, FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    myFile.close();
  }
}
void f_read(String file) {
  myFile = SD.open(file);
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
}
