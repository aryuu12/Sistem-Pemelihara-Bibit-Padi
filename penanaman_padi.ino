#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 13
#define DHTTYPE DHT11

#define RELAY_KIPAS 8
#define RELAY_POMPA 9

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

bool kipasNyala = false;
bool pompaNyala = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();

  pinMode(RELAY_KIPAS, OUTPUT);
  pinMode(RELAY_POMPA, OUTPUT);

  digitalWrite(RELAY_KIPAS, LOW);
  digitalWrite(RELAY_POMPA, LOW);

  delay(100);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("suhu : ");
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("kelembapan : ");
  lcd.print(h);
  lcd.print("%");

  // Kontrol kipas: nyala kalau kelembapan di atas 80%
  if (h > 80) {
    digitalWrite(RELAY_KIPAS, HIGH);
    kipasNyala = true;
  } else {
    digitalWrite(RELAY_KIPAS, LOW);
    kipasNyala = false;
  }

  // Kontrol pompa kabut: nyala kalau kelembapan di bawah 60%
  if (h < 60) {
    digitalWrite(RELAY_POMPA, HIGH);
    pompaNyala = true;
  } else {
    digitalWrite(RELAY_POMPA, LOW);
    pompaNyala = false;
  }

  // Virtual Terminal mengikuti kondisi motor
  if (kipasNyala) {
    Serial.println("Kipas: HIDUP");
  } else {
    Serial.println("Kipas: MATI");
  }

  if (pompaNyala) {
    Serial.println("Pompa kabut: HIDUP");
  } else {
    Serial.println("Pompa kabut: MATI");
  }

  if (!kipasNyala && !pompaNyala) {
    Serial.println("Kondisi: NORMAL (kipas & pompa mati)");
  }

  Serial.println("----------------");

  delay(1000);
}