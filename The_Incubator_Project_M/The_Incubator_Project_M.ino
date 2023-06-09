#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <L298N.h>

// Pins
// LCD Pins A4, A5
// DHT pin 2
// Motor Pins 7, 8, 9
// Heater pin 4
// Limit SwitcH Pin 3
// Fan Switch If Any 11


//Timer
unsigned long int currentTime = 0;
unsigned long int previousTime = 0;
unsigned long int angleTimer;

// LCD config.
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd set to 16x2 using address 0x27

//DTH configuration
#define Type DHT11
int sensePin = 2;
DHT HT(sensePin, Type);
float humidity, tempC;
int htdelay = 1000;
int dt = 1000, dt2 = 5000;

//motor Driver
const unsigned int IN1 = 7;
const unsigned int IN2 = 8;
const unsigned int EN = 9;
//ONE MOTOR INSTANCE
L298N motor(EN, IN1, IN2);
int dtr = 3000;

//Heat Pins
const unsigned int heatPin = 4;

////Limit Switch
//       const unsigned int limitPin = 3;
//       int limitOutput ;

//Fan Switch
const unsigned int fanPin = 11;


void setup() {
  // put your setup code here, to run once:
  //  Serial.begin(9600);
  pinMode(heatPin, OUTPUT); // heater pin
  pinMode(fanPin, OUTPUT); //fan pin
  //lcd display
  lcd.init();
  lcd.backlight();
  lcd.print("Starting ");
  lcd.setCursor(0, 1);
  lcd.print("Incubator....");
  delay(dt2);
  // HT sensor
  HT.begin();
  delay(htdelay);
  // motor sensor speed
  motor.setSpeed(255);



}

void loop() {
  // put your main code here, to run repeatedly:
  readTempHumidity();
  tempHumidity();
  currentTime = millis();

  if (currentTime - previousTime >= 12000) {
    currentTime = millis();
    previousTime = millis();
    while(currentTime - previousTime <= 12000){
        motor.backward();
        printInfo2();
        readTempHumidity();
        tempHumidity();
        currentTime = millis();
  }
     currentTime = millis();
     previousTime = millis();
  while (currentTime - previousTime <= 2000) {
        motor.stop();
        currentTime = millis();
  }
      currentTime = millis();
      previousTime = millis();
  while (currentTime - previousTime <= 12000) {
        motor.forward();
        printInfo();
        readTempHumidity();
        tempHumidity();
        currentTime = millis();
  }
     currentTime = millis();
     previousTime = millis();
  while (currentTime - previousTime <= 2000) {
        motor.stop();
        currentTime = millis();
  }
    previousTime = millis();
}
 
}
void readTempHumidity() {
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hmd:");
  lcd.print(humidity);
//  lcd.print(",");
//  lcd.setCursor(0, 0);
  lcd.print(",Tmp:");
  lcd.print(tempC);
  delay(dt);
}

void tempHumidity() {
  if (tempC <= 36) {
    digitalWrite(heatPin, HIGH);

  }

  if ( tempC >= 37.5) {
    digitalWrite(heatPin, LOW);
  }

  if (humidity <= 42.00) {
    digitalWrite(fanPin, LOW);
  }
  if (humidity >= 47.00) {
    digitalWrite(fanPin, HIGH);
  }

}

void printInfo() {
  lcd.setCursor(0, 1);
  lcd.print("Turning -->");
}
void printInfo2() {
  lcd.setCursor(0, 1);
  lcd.print("Turning <--");
}
