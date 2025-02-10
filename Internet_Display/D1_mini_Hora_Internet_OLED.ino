/////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "ssid" const char* password = "password";

#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

#define led 2  // OJO: En el D1 mini, el led interno, en el GPIO2 se activa en Bajo, es decir, en LOW se enciende y en HIGH de apaga.
uint32_t tmp_led = 0;
int contador = 0;
uint32_t tmp_contador = 0;
char cont_str[5];

#define hora_interval 60000
uint32_t tmp_hora = 0;

////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, !digitalRead(led));
    delay(200);
  }
  digitalWrite(led, HIGH);

  init_display();
  init_Wifi();
  delay(2000);
  obtenerHora();
}

void loop() {
  func_led();
  func_contador();
  if (millis() > tmp_hora + hora_interval) {
    tmp_hora = millis();
    obtenerHora();
  }
}


/////////////////////////////////////////////////////

void init_display() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);

  display.setCursor(5, 5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.write("UP THE IRONS!");
  display.display();
}

void init_Wifi() {
  WiFi.begin(ssid, password);
  display.setCursor(5, 20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.write("Conectando ...");
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  display.drawRect(32, 29, 64, 28, SSD1306_WHITE);
  display.setCursor(5, 20);
  display.write("CONECTADO     ");
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
    display.setCursor(100, 5);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    sprintf(cont_str, "%04d", contador);
    display.print(cont_str);
    display.display();
    //Serial.println(cont_str);
    contador++;
  }
}


void obtenerHora() {
  delay(2000);
  Serial.println("Obteniendo la hora.");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();
    http.begin(client, "https://timeapi.io/api/time/current/zone?timeZone=Europe%2FMadrid");
    int httpCode = http.GET();
    Serial.println(httpCode);

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      String fecha = obtenerValorJSON(payload, "\"date\":\"", "\",");
      String hora = obtenerValorJSON(payload, "\"time\":\"", "\",");
      Serial.print("Hora actual UTC: ");
      Serial.print(fecha + " - " + hora);
      display.setCursor(35, 35);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      display.print(hora);
      display.display();
    } else {
      Serial.println("Error en la solicitud HTTP");
    }
    http.end();
  } else {
    Serial.println("No hay conexión Wi-Fi");
  }
}


String obtenerValorJSON(String json, String claveInicio, String claveFin) {
  int inicio = json.indexOf(claveInicio);
  if (inicio == -1) return "";

  inicio += claveInicio.length();
  int fin = json.indexOf(claveFin, inicio);
  if (fin == -1) return "";

  return json.substring(inicio, fin);
}