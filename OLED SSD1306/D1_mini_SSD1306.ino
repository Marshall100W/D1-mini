/////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

#define led 2  // OJO: En el D1 mini, el led interno, en el GPIO2 se activa en Bajo, es decir, en LOW se enciende y en HIGH de apaga.
uint32_t tmp_led = 0;
int contador = 0;
uint32_t tmp_contador = 0;
char cont_str[5];
////////////////////////////////////////////////////////


void setup() {
  pinMode(led, OUTPUT);
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, !digitalRead(led));
    delay(200);
  }
  digitalWrite(led, HIGH);

  init_display();
  delay(1000);
}

void loop() {
  func_led();
  func_contador();
}


/////////////////////////////////////////////////////

void init_display() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.drawRect(32, 20, 64, 32, SSD1306_WHITE);
  display.setCursor(5, 5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.write("UP THE IRONS!");
  display.display();
}


void func_led() {
  if (millis() > tmp_led + 500) {
    tmp_led = millis();
    digitalWrite(led, !digitalRead(led));
  }
}


void func_contador() {
  if (millis() > tmp_contador + 1000) {
    tmp_contador = millis();
    display.setCursor(40, 30);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    sprintf(cont_str, "%04d", contador);
    display.print(cont_str);
    display.display();
    contador++;
  }
}
