// Include the library code:
#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// For ultrasonic sensor
int distanceThreshold = 100; // Adjusted threshold for door opening
int cm = 0;
int inches = 0;

// For relay control
int releNO = 13;
int inputPir = 8;
int val = 0;
int resuldoSensorLDR;
int sensorLDR = A0;

// For gas sensor
int const PINO_SGAS = A1;

// Buzzer pin
int buzzerPin = 9;

// Function to read distance from ultrasonic sensor
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  pinMode(releNO, OUTPUT);
  pinMode(inputPir, INPUT);
  pinMode(sensorLDR, INPUT);
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  Serial.begin(9600);
}

void loop() {
  // Measure the distance in cm
  cm = 0.01723 * readUltrasonicDistance(7, 6);
  // Convert to inches by dividing by 2.54
  inches = (cm / 2.54);
  
  lcd.setCursor(0, 0); // Sets the location for displaying distance on LCD
  lcd.print("D:"); // Prints string "D:" on the LCD
  lcd.print(cm); // Prints the distance value
  lcd.print("cm");
  delay(10);

  // Automatic door control based on distance
  if (cm < distanceThreshold) { // If within threshold, open door
    digitalWrite(releNO, HIGH); // Open door
    lcd.setCursor(0, 1);
    lcd.print("Door: Open  ");
    delay(5000); // Keep the door open for 5 seconds
    digitalWrite(releNO, LOW); // Close door
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Door: Closed");
  }
  
  // Read PIR and LDR values
  val = digitalRead(inputPir);
  resuldoSensorLDR = analogRead(sensorLDR);

  // Light control based on LDR and PIR
  if (resuldoSensorLDR < 600) { // Dim light condition
    if (val == HIGH) { // If motion is detected
      digitalWrite(releNO, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("L: On ");
      delay(5000);
    } else {
      digitalWrite(releNO, LOW);
      lcd.setCursor(0, 1);
      lcd.print("L: Off");
      delay(300);
    }
  } else {
    digitalWrite(releNO, LOW);
    Serial.println(resuldoSensorLDR);
    delay(500);
  }

  // Gas level monitoring
  int gasLevel = analogRead(PINO_SGAS);
  
  lcd.setCursor(8, 0); // Display gas level on LCD
  if (gasLevel <= 85) {
    lcd.print("G:Low ");
  } else if (gasLevel <= 120) {
    lcd.print("G:Med ");
  } else if (gasLevel <= 200) {
    lcd.print("G:High");
    
    // Buzzer control: Activate for 5 seconds when gas level is High
    digitalWrite(buzzerPin, HIGH);
    delay(5000);
    digitalWrite(buzzerPin, LOW);

  } else if (gasLevel <= 300) {
    lcd.print("G:Ext ");
  }
  
  delay(250); // Delay for smooth operation
}
