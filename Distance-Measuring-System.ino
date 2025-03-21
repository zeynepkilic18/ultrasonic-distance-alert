#include <Arduino.h>

const int trigPin = 3; //Trig 3. pine bağlı
const int echoPin = 4; //Echo 4. pine bağlı
const int buzzerPin = 8;  //Buzzer + ucu 8.pine bağlı

float getDistance() {  //Mesafeyi hesapla
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); //Sensörün düzgün çalışması için önerilen bir hazırlık süresi

  digitalWrite(trigPin, HIGH); //Sensör ultrasonik dalga gönderir.
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //Sinyal gönderimi biter

  long duration = pulseIn(echoPin, HIGH, 30000); //Sensör, gönderdiği dalganın engel tarafından verilen sürede geri dönmesini bekler.
  float distance = duration / 58.0; //Sesin havadaki hızı yaklaşık 340 m/s olduğundan bu çeviri yapılır, HC-SR04'ün veri sayfasından alınan sabittir.

  return distance / 100.0;  //Mesafeyi metre cinsine çevir
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); //Dışarıya sinyal çıkarır
  pinMode(echoPin, INPUT); //Geri dönen sinyali alır ve Arduino bu pini dinler
  pinMode(buzzerPin, OUTPUT); //Arduino bu pin aracılığıyla buzzer’a sinyal gönderir.
}

void loop() {
  float distance = getDistance();
  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.println(" m");

  if (distance <= 0.5) {
    //0.5m - 0m: Sürekli ötsün
    digitalWrite(buzzerPin, HIGH);
    delay(100);  //Hemen tekrar ölçüm için(kısa bir gecikme süresi)
  } 
  else if (distance <= 1.0) {
    //1m - 0.5m: 1 saniyede 1 kez ötsün
    digitalWrite(buzzerPin, HIGH);
    delay(200);  
    digitalWrite(buzzerPin, LOW);
    delay(800);   
  } 
  else if (distance <= 2.0) {
    //2m - 1m: 3 saniyede 1 kez ötsün
    digitalWrite(buzzerPin, HIGH); //Ötme modu
    delay(200);   //200ms öt
    digitalWrite(buzzerPin, LOW); //Sessizlik modu
    delay(2800);  //2.8 saniye sessiz
  } 
  else {
    //2 metreden uzaksa: 5 saniyede 1 kez ötsün
    digitalWrite(buzzerPin, HIGH);
    delay(200);   
    digitalWrite(buzzerPin, LOW);
    delay(4800);  
  }
}
