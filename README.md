# 🌡️ Monitoramento de Ambiente com Arduino (Simulado no Wokwi)

Este projeto simula, utilizando a plataforma [Wokwi](https://wokwi.com/), um sistema embarcado de monitoramento ambiental com Arduino. Ele detecta **luminosidade**, **temperatura** e **umidade** e responde com sinais visuais (LEDs), sonoros (buzzer) e mensagens no **Display LCD**.

---

### 👥 Participantes
- Felipe Godoi
- Guilherme Augusto
- Raphael Taketa
- Victor Guimarães

---

## 📦 Descrição do Projeto

Este projeto simula um Sistema de Monitoramento Ambiental para Vinheria Agnello, desenvolvido na plataforma Wokwi com o uso do Arduino Uno. O objetivo é garantir o controle adequado de luminosidade, temperatura e umidade, fatores essenciais para a conservação e envelhecimento ideal dos vinhos.

Através de sensores integrados, o sistema realiza leituras periódicas do ambiente da adega e reage automaticamente conforme os seguintes critérios:

 - Exibe informações em um display LCD com mensagens claras sobre as condições do ambiente;

 - Ativa LEDs indicadores (verde, amarelo e vermelho) para facilitar a visualização do status;

 - Emite um alarme sonoro (buzzer) em casos de condições críticas, como excesso de luz ou temperatura fora da faixa recomendada.

Os dados apresentados são baseados na média de 5 leituras e atualizados a cada 5 segundos, garantindo maior precisão. Este sistema é uma solução prática e acessível para automatizar o controle climático de vinherias, protegendo a qualidade dos rótulos armazenados e minimizando riscos de degradação.

---

## ✅ Funcionalidades

### 🌞 Luminosidade
- **Ambiente Escuro**: LED **Verde** aceso.
- **Meia luz**: LED **Amarelo** aceso + mensagem `Ambiente a meia luz` no display.
- **Ambiente Muito Claro**:
  - LED **Vermelho** aceso;
  - Mensagem `Ambiente muito claro` no display;
  - Buzzer ligado continuamente.

### 🌡️ Temperatura
- **Entre 10°C e 15°C**:
  - Mensagem `Temperatura OK` no display;
  - Exibe valor da temperatura.
- **Abaixo de 10°C**:
  - LED Amarelo aceso;
  - Buzzer ligado;
  - Mensagem `Temp. Baixa` no display.
- **Acima de 15°C**:
  - LED Amarelo aceso;
  - Buzzer ligado;
  - Mensagem `Temp. Alta` no display.

### 💧 Umidade
- **Entre 50% e 70%**:
  - Mensagem `Umidade OK` no display;
  - Exibe valor da umidade.
- **Abaixo de 50%**:
  - LED Vermelho aceso;
  - Buzzer ligado;
  - Mensagem `Umidade. Baixa` no display.
- **Acima de 70%**:
  - LED Vermelho aceso;
  - Buzzer ligado;
  - Mensagem `Umidade. Alta` no display.

---

## ⏱️ Lógica de Atualização

- A cada **5 segundos**:
  - São feitas **5 leituras** dos sensores (com média aritmética);
  - Os dados médios de temperatura, umidade e luminosidade são exibidos;
  - A lógica de LEDs, buzzer e display é atualizada conforme as condições.

---

## 🔧 Componentes Utilizados (Simulados)

| Componente         | Descrição                        |
|--------------------|----------------------------------|
| Arduino Uno        | Placa base                       |
| DHT22              | Sensor de temperatura e umidade  |
| Fotoresistor (LDR) | Sensor de luminosidade           |
| LEDs               | Verde, Amarelo, Vermelho         |
| Buzzer             | Emissor de som                   |
| LCD 16x2 I2C       | Display para mensagens           |
| Resistores         | Para LDR e LEDs                  |

---

## 📦 Bibliotecas Necessárias

- `DHT sensor library` (Adafruit)
- `Adafruit Unified Sensor`
- `LiquidCrystal_I2C`


---

## ▶️ Como Simular

1. Acesse o link do projeto: [Simulação no Wokwi](https://wokwi.com/projects/430778567016292353)
2. Clique em **"Start Simulation"**.
3. Observe os LEDs, o display LCD e o buzzer responderem às condições ambientais.

## 🔌 Conexões

| Componente       | Pino Arduino |
|------------------|--------------|
| DHT22 (Data)     | D2           |
| Fotoresistor     | A0           |
| LED Verde        | D3           |
| LED Amarelo      | D4           |
| LED Vermelho     | D5           |
| Buzzer           | D6           |
| LCD I2C SDA      | A4           |
| LCD I2C SCL      | A5           |

Placa: Arduino Uno


## 📁 Código

```cpp
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
```

---

## 🛠️ Observações Técnicas

- O LDR é lido via `analogRead()` e mapeado para escala de 0 a 100%.
- As médias são calculadas com 5 leituras consecutivas, com atraso de 200ms entre elas.
- O buzzer é controlado via `tone()` para emitir som contínuo.

---

## ⚠️ Dificuldades Encontradas

Durante o desenvolvimento do projeto, a principal dificuldade enfrentada foi a manipulação e escrita de informações no display LCD. Foi necessário ajustar corretamente os tempos de atualização e o espaçamento das mensagens, além de evitar conflitos entre leituras simultâneas e o uso do lcd.clear(), que, se usado de forma errada, podia causar efeitos indesejados, como piscadas ou mensagens cortadas. Para resolvermos esses problemas, foi necessário muitos testes e aprendizado através de vídeos do youtube explicando mais sobre o display.

## 📽️ Demonstração

- 💻 [Simulação no Wokwi](https://wokwi.com/projects/430778567016292353)