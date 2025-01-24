///////////////////////////////////


#define led 2  // OJO: En el D1 mini, el led interno, en el GPIO2 se activa en Bajo, es decir, en LOW se enciende y en HIGH de apaga.
#define pir 0
int tmp_pir = 0;
bool stat_pir = false;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, !digitalRead(led));
    delay(200);
  }
  digitalWrite(led, HIGH);
  delay(3000);
}


void loop() {
  func_pir();
}


void func_pir() {
  if (millis() > tmp_pir + 100) {
    tmp_pir = millis();
    stat_pir = digitalRead(pir);
    digitalWrite(led, !stat_pir);
  }
}
