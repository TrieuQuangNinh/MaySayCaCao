#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define dieuChinhNhietDo 32   
#define dieuChinhDoAm 33
#define dienTro 0   // 
#define quatHut 4 // 
#define DHTPIN 27 // doi chan 25 thanh 27

// 2 Chan 15 va 16 chua su dung
#define chanSo4 15    
#define chanSo16 16  
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal_I2C lcd(0x27, 20, 4);

boolean trangThaiDienTro = false, trangThaiQuatHut = false; 

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(dienTro, OUTPUT);
  pinMode(quatHut, OUTPUT);

  digitalWrite(dienTro, LOW); 
  digitalWrite(quatHut, LOW);
  digitalWrite(chanSo4, LOW);
  digitalWrite(chanSo16, LOW);
  //delay(5000);
}

void loop() {
  lcd.clear(); 
  readAndDisplayData();
  delay(3000);
}

void readAndDisplayData() {
  float tempC = dht.readTemperature();
  float humi  = dht.readHumidity(); 
  long giaTriNhietDo = analogRead(dieuChinhNhietDo);
  long giaTriDoAm = analogRead(dieuChinhDoAm);
  int targetTemp = map(giaTriNhietDo, 0, 4095, 0, 100); 
  int targetHumi = map(giaTriDoAm, 0, 4095, 0, 100); 
  float tempDiff = targetTemp - tempC; 
  float humiDiff = targetHumi - humi; 

  Serial.println("////////////////");
  Serial.println("ND CB");
  Serial.println(tempC);
  Serial.println(humi);
  // Serial.println("ND MM");
  // Serial.println(targetTemp);
  // Serial.println(targetHumi);  
  // Serial.println("Sai so");
  // Serial.println(tempDiff);
  // Serial.println(humiDiff);  


  // DOC NHIET DO
  lcd.setCursor(0, 0); 
  lcd.print("NHIET DO: ");
  lcd.print(tempC); 
  lcd.print((char)223); 
  lcd.print("C");

  // DOC DO AM
  lcd.setCursor(0, 1);  
  lcd.print("DO AM   : ");
  lcd.print(humi);     
  lcd.print("%");

  // SET NHIET DO
  lcd.setCursor(0, 2);
  lcd.print("ND MONG MUON: "); 
  lcd.print(targetTemp);
  lcd.print((char)223);
  lcd.print("C");
  //lastTargetTemp = targetTemp;

  // SET DO AM
  lcd.setCursor(0, 3);
  lcd.print("DA MONG MUON: "); 
  lcd.print(targetHumi);
  lcd.print("%");
  //lastTargetHumi = targetHumi;

  // Dieu khien dien tro
  if (tempDiff > 2) {  
    digitalWrite(dienTro, HIGH);
    trangThaiDienTro = true; 
  } 
  else if (tempDiff < -2) { 
    digitalWrite(dienTro, LOW);
    trangThaiDienTro = false; 
  }

  // Dieu khien do am (Quat hut dang sau)
  if (humiDiff < -3) {  
    digitalWrite(quatHut, HIGH);
    trangThaiQuatHut = true; 
  } 
  else if (humiDiff > -3) { 
    digitalWrite(quatHut, LOW);
    trangThaiQuatHut = false; 
  }  
}