#include <Adafruit_Sensor.h>       
#include <DHT.h>                  
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//#define DHTTYPE    DHT11                     
#define DHTTYPE    DHT22                       
#define DHTPIN 2                                
DHT_Unified dht(DHTPIN, DHTTYPE);              
uint32_t delayMS;                             


#define col 16
#define lin  2
#define ende 0x27
LiquidCrystal_I2C lcd(ende, col, lin);


const int fotoresistor = A0;
const int ledVerde = 3;
const int ledAmarelo = 4;
const int ledVermelho = 5;
const int buzzer = 6;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  float somaTemp = 0, somaUmidade = 0, somaLuz = 0;

  for (int i = 0; i < 5; i++) {
    sensors_event_t tempEvent, humEvent;
    dht.temperature().getEvent(&tempEvent);
    dht.humidity().getEvent(&humEvent);

    float temp = !isnan(tempEvent.temperature) ? tempEvent.temperature : 0;
    float hum = !isnan(humEvent.relative_humidity) ? humEvent.relative_humidity : 0;
    float luz = analogRead(fotoresistor);

    somaTemp += temp;
    somaUmidade += hum;
    somaLuz += map(luz, 1023, 0, 0, 100);

    delay(200);
  }

  float tempMed = somaTemp / 5.0;
  float umiMed = somaUmidade / 5.0;
  float luzMed = somaLuz / 5.0;

  // Reset
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(buzzer, LOW);

  // --- LUMINOSIDADE ---
  noTone(buzzer);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luz: ");
  lcd.print((int)luzMed);
  lcd.print("%");

  lcd.setCursor(0, 1);
  if (luzMed < 30) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
    lcd.print("Amb. Escuro");
  } else if (luzMed >= 30 && luzMed < 70) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
    lcd.print("Amb. a meia luz");
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, HIGH);
    lcd.print("Amb. muito claro");
    tone(buzzer, 300);
    
  }

  delay(5000);

  // --- TEMPERATURA ---
  noTone(buzzer);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempMed, 1);
  lcd.print("C");

  lcd.setCursor(0, 1);
  if (tempMed >= 10 && tempMed <= 15) {
    lcd.print("Temperatura OK");
  } else if (tempMed < 10) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, HIGH);
    lcd.print("Temp. Baixa");
    tone(buzzer, 300);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, HIGH);
    lcd.print("Temp. Alta");
    tone(buzzer, 300);
  }

  delay(5000);

  // --- UMIDADE ---
  noTone(buzzer);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umid: ");
  lcd.print(umiMed, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  if (umiMed >= 50 && umiMed <= 70) {
    lcd.print("Umidade OK");
  } else if (umiMed < 50) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, HIGH);
    lcd.print("Umidade. Baixa");
    tone(buzzer, 300);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, HIGH);
    lcd.print("Umidade. Alta");
    tone(buzzer, 300);
  }

  delay(5000); 
}