#include <SoftwareSerial.h>
#include <DHT.h>

const uint8_t PIN_DHT = 2;
const uint8_t PIN_LED = 13;
const uint8_t PIN_RX  = 10;   // terima dari TX ESP32
const uint8_t PIN_TX  = 11;   // kirim ke RX ESP32 (lewat pembagi tegangan)

const unsigned long INTERVAL_BACA = 2000;

DHT dht(PIN_DHT, DHT11);
SoftwareSerial esp(PIN_RX, PIN_TX);

bool  modeAuto = false;
bool  ledOn    = false;
float ambang   = 30.0;
float suhu     = 0;
float lembap   = 0;
bool  sensorOk = false;

unsigned long waktuBaca = 0;

char buf[24];
uint8_t idx = 0;

void setLed(bool on) {
  ledOn = on;
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

void aturOtomatis() {
  if (modeAuto && sensorOk) {
    setLed(suhu <= ambang);
  }
}

void bacaSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  sensorOk = !(isnan(t) || isnan(h));
  if (sensorOk) {
    suhu = t;
    lembap = h;
  }
}

void kirimStatus() {
  esp.print(F("DATA,"));
  esp.print(suhu, 1);   esp.print(',');
  esp.print(lembap, 0); esp.print(',');
  esp.print(ledOn ? 1 : 0); esp.print(',');
  esp.print(modeAuto ? F("AUTO") : F("MANUAL")); esp.print(',');
  esp.print(ambang, 1); esp.print(',');
  esp.println(sensorOk ? 1 : 0);

  Serial.print(F("Suhu=")); Serial.print(suhu, 1);
  Serial.print(F(" C, Lembap=")); Serial.print(lembap, 0);
  Serial.print(F("%, LED=")); Serial.print(ledOn ? F("ON") : F("OFF"));
  Serial.print(F(", Mode=")); Serial.print(modeAuto ? F("AUTO") : F("MANUAL"));
  Serial.print(F(", Ambang=")); Serial.print(ambang, 1);
  Serial.println(sensorOk ? F("") : F("  [DHT11 GAGAL DIBACA]"));
}

void prosesPerintah(const char* c) {
  if (strcmp(c, "MODE:MANUAL") == 0) {
    modeAuto = false;
  } else if (strcmp(c, "MODE:AUTO") == 0) {
    modeAuto = true;
    aturOtomatis();
  } else if (strcmp(c, "LED:ON") == 0) {
    if (modeAuto) setLed(true);
  } else if (strcmp(c, "LED:OFF") == 0) {
    if (modeAuto) setLed(false);
  } else if (strncmp(c, "TH:", 3) == 0) {
    ambang = atof(c + 3);
    aturOtomatis();
  } else {
    return;
  }
  kirimStatus();
}

void bacaPerintah() {
  while (esp.available()) {
    char ch = esp.read();
    if (ch == '\n') {
      buf[idx] = '\0';
      if (idx > 0) prosesPerintah(buf);
      idx = 0;
    } else if (ch != '\r' && idx < sizeof(buf) - 1) {
      buf[idx++] = ch;
    }
  }
}

void setup() {
  Serial.begin(9600);
  esp.begin(4800);
  dht.begin();
  pinMode(PIN_LED, OUTPUT);
  setLed(false);
  Serial.println(F("Uno siap. Mode awal: MANUAL, LED mati."));
}

void loop() {
  bacaPerintah();

  if (millis() - waktuBaca >= INTERVAL_BACA) {
    waktuBaca = millis();
    bacaSensor();
    aturOtomatis();
    kirimStatus();
  }
}
