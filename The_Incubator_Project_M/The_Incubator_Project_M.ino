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
// Fan Switch If Any 5


//Timer
      unsigned long currentTime = millis();
      unsigned long previousTime = 0;
      unsigned long angleTimer;

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

//Limit Switch
       const unsigned int limitPin = 3;

//Fan Switch
       const unsigned int fanPin = 5;       

             
void setup() {
  // put your setup code here, to run once:
  //  Serial.begin(9600);
     // pinMode(sensePin, INPUT); //dht pin
      pinMode(limitPin, INPUT); //limit pin
      pinMode(heatPin, OUTPUT); // heater pin
      pinMode(fanPin, OUTPUT); //fan pin
  //lcd display
      lcd.init();
      lcd.backlight();
      lcd.print("Starting ");
      lcd.setCursor(0,1);
      lcd.print("IncuBator....");
      delay(dt2);
  // HT sensor
      HT.begin();
      delay(htdelay);
  // motor sensor speed
      motor.setSpeed(255); 
  // fan switch always on
   digitalWrite(fanPin, HIGH);



  //Rotating the eggs upon powering On.
    if(currentTime == 10 && limitPin == 1){
      motor.forward();
      while (limitPin != 0){
        
      }   
      motor.stop();
      
    }

    if (currentTime == 10 && limitPin != 1){
      while(currentTime < 15000){ //hardcode time for it to reach 180 degrees
         motor.backward();
      }
          motor.stop();
    }
   
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humd: ");
  lcd.print(humidity);
  lcd.print(" ");
//  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(tempC);
  delay(dt);

//Motor control with timer.
//120,000 milliseconds Equals 2 minutes
  
  if(currentTime - previousTime == 120000 && limitPin != 0){ 
         while(limitPin != 1){  // when the limitSwitch is Triggered
              motor.forward();
              printInfo();
        }  
            motor.stop();
            previousTime = currentTime;
      }
  if(currentTime - previousTime == 120000 && limitPin == 0){
    motor.backward();
    printInfo2();
    angleTimer= currentTime + 15000;
    while(currentTime < angleTimer){
      
    }
        motor.stop();
        previousTime = currentTime;
  }

//bulb heater control
   if (humidity<=55 || tempC <= 36){
      digitalWrite(heatPin, HIGH);
      
    }
    if(humidity>=60 || tempC >= 37.5){
      digitalWrite(heatPin, LOW);  
    }


}


  void printInfo() {
//    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Turning -->");
  }
   void printInfo2() {
//    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Turning <--");
  }
