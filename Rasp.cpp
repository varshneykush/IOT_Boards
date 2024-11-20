#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define DHT sensor type and pin
#define DHTPIN 15
#define DHTTYPE DHT22

// Pin Definitions
#define PIR_PIN 14
#define TRIG_PIN 13
#define ECHO_PIN 12
#define LDR_PIN A0
#define BUZZER_PIN 16
#define RELAY_PIN 17

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD (update the I2C address based on scanner results)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function to measure distance using Ultrasonic Sensor
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}

void setup() {
  // Initialize serial monitor
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LCD
  lcd.begin(16, 2); // Specify 16 columns and 2 rows
  lcd.backlight();
  
  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Welcome Message
  lcd.setCursor(0, 0);
  lcd.print("System Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read sensors
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int pirState = digitalRead(PIR_PIN);
  int ldrValue = analogRead(LDR_PIN);
  int distance = measureDistance();

  // Display values on LCD
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temp, 1);
  lcd.print("C H: ");
  lcd.print(humidity, 1);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm L:");
  lcd.print(ldrValue);

  // Log readings for debugging
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Distance: ");
  Serial.print(distance);
  Serial.print("cm, LDR: ");
  Serial.println(ldrValue);

  // Check conditions and trigger alarms
  if (temp > 35 || humidity < 20 || distance < 10 || ldrValue < 200 || pirState == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Exit State!");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(2000); // Wait for 2 seconds before the next loop
}
