///////////////////////////////////
#include <Wire.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

#define led 2  // OJO: En el D1 mini, el led interno, en el GPIO2 se activa en Bajo, es decir, en LOW se enciende y en HIGH de apaga.
#define pir 0
int tmp_pir = 0;
bool stat_pir = false;

int contador = 0;
uint32_t tmp_contador = 0;
char buffer[20];
bool alarma = false;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
  inicio_oled();
  inicio_led();
  delay(1000);
  Serial.println("UP THE IRONS!");
}


void loop() {
  func_pir();
  func_contador();
}


void inicio_led() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, !digitalRead(led));
    delay(200);
  }
  digitalWrite(led, HIGH);
}


void inicio_oled() {
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 16, 1);
  oled.drawRect(0, 17, 128, 47, 1);
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(25, 5);
  oled.print(F("UP THE IRONS!"));
  oled.display();
}


void func_pir() {
  if (millis() > tmp_pir + 100) {
    tmp_pir = millis();
    stat_pir = digitalRead(pir);
    digitalWrite(led, !stat_pir);
    if (stat_pir && !alarma) {
      Serial.println("ALARMA!!!");
      oled.setTextSize(2);
      oled.setTextColor(1);
      oled.setCursor(15, 25);
      oled.print(F("ALARMA!!!"));
      oled.display();
      alarma = true;
    }
    if (!stat_pir) {
      oled.fillRect(13, 25, 110, 14, 0);
      oled.display();
      alarma = false;
    }
  }
}


void func_contador() {
  if (millis() > tmp_contador + 1000) {
    tmp_contador = millis();
    oled.setTextSize(1);
    oled.setTextColor(1, 0);
    sprintf(buffer, "%04d", contador);
    oled.setCursor(95, 52);
    oled.print(String(buffer));
    oled.display();
    Serial.println(buffer);
    contador++;
  }
}
